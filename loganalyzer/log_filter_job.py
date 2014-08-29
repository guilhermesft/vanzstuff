#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 Toda a aplicação foi desenvolvida usando threads. Onde cada thread executa tem uma terefa especifica. São elas:
 * SplitLogFileTask: Thread que fica observando um diretório em busca de novos arquivos de log. Entado um arquivos eh
                     feito um split dele em chunk menores. Esses chunk são colocados em um fila para processamento futuro
 * ChunkProcessor: Thrad que processa os chunk gerados. Na parte de processamento, eh gerado um dicionário onde a chave eh
 		   o valor do userid e o valor eh uma lista dos registro de log para o respectivo userid ordenado pela data e hora.
		   Os dicionários gerados são colocados em um segunda fila para processamento.
 * OutPutWriter: Essa thread corresponde a parte final do processamento. Ela é responsável em gravar os resgistros separados e
 		      ordenados pelo chunk processor em arquivo.
'''

import threading
import os
import re
import datetime
import time
import sets
import logging
import Queue
import tempfile
import glob

#Thread que pega um arquivo de log e quebra em chunk menores para processamento
class SplitLogFileTask(threading.Thread):

	def __init__(self, dir, dir_lock, output_queue, files_done):
		super(SplitLogFileTask, self).__init__()
		#diretório que a thread monitora
		self.dir = dir
		#lock de acesso para o diretório
		self.dir_lock = dir_lock
		#fila onde será colocados os chunks
		self.output_queue = output_queue
		#conjunto que contem todos os arquivos que já forem processados
		self.files_done = files_done
		#flag que indica se a thread deve morrer
		self.dead = False

	def  run(self):
		logger = logging.getLogger('log')
		logger.debug('%s', "Iniciando execução", extra={'type': 'SPLITTER'})
		#se o arquivo de log especificado não existe, não faz nada. =(
		if not os.path.exists(self.dir):
			logger.debug('%s', "Diretório não encontrado", extra={'type':'SPLITTER'})
			return
		#o arquivo existe, agora vamos ler e gerar os chunk para processamento
		#para cada 100 registros encontrados nos arquivos sera gerado um outro chunck
		while not self.dead:
			#no momento de acessar o diretório tem que ser uma thread por vez
			self.dir_lock.acquire()
			#cria um conjunto com todos os arquivos de log
			all_log_files = set(glob.glob(self.dir + os.sep + 'log-*'))
			#da lista de todos os arquivos de log, retira o que jpa foi processado
			all_log_files.difference_update(self.files_done)
			#para não precisar tratar a exception, vou verificar se tem algo antes
			if len(all_log_files) == 0:
				self.dir_lock.release()
				#jah que todos os arquivos forem processador. Fim de papo
				self.dead = True
				logger.debug('%s', "Todos os arquivos processados", extra={'type':'SPLITTER'})
				continue
			#pega um arquivo para processar
			log_file = all_log_files.pop()
			#adiciona o arquivo no conjunto de processados. Assim outras thread não vão mais pega ele
			self.files_done.add(log_file)
			#libera o diretório
			self.dir_lock.release()
			with open(log_file, 'r') as file:
				logger.debug('Processando: %s ', file.name, extra={'type':'SPLITTER'})
				chunk = []
				for entry in file:
					#adiciona o regritro de log lido no chunk que esta sendo gerado
					chunk.append(entry)
					#verifica se o chunk não esta cheio
					if len(chunk) == 100:
						#o chunk esta cheio, agora coloca ele na fila para processamento
						#como a queue eh thread safe não é necessário nenhum mecanismo que sincronia extra
						self.output_queue.put(chunk)
						#limpa o chunk para a próxima interação
						chunk = []
				logger.debug("Terminou de processar: %s ", file.name, extra={'type':'SPLITTER'})
		logger.debug('%s', "Finalizado", extra={'type':'SPLITTER'})
	 	 	#no momento de acessar o diretório tem que ser uma thread por vez

#Thread que pega os chunk separados pela etapa anterior e os separa por userid
class ChunkProcessor(threading.Thread):

	def __init__(self, input_queue, output_queue):
		super(ChunkProcessor, self).__init__()
		#fila onde estão os chunk pendentes
		self.input = input_queue
		#fila onde eh colocado o resultado do processamento
		self.output = output_queue
		#flag que indica se a thread deve morrer
	 	self. dead = False

	def run(self):
		logger = logging.getLogger('log')
		#enquando ninguém matar a thread... toca o barco.
		while not self.dead:
			try:
				#tenta pegar um chunk na fila de chunk pendentes. Se não retornar nada em 15 segundos, aborta
				chunk = self.input.get(True, 15)
			except Queue.Empty:
				#não tem nada na fila. Vai de novo!
				continue;
			#expressão regular utilizada para pegar o valor do biscoito do registro de log
			regex = re.compile('"userid=(?P<biscoito>.+)"')
			output_dict = {}
			for entry in chunk:
				#procura pelo cookie no registro do log
				cookie = regex.search(entry).group('biscoito')
				if cookie is not None:
					#verifica se esse cookie jah não foi encontrado no chunk processado
					if not output_dict.has_key(cookie):
						#esse userid ainda não foi processado, então precisamos colocar ele no dicionario
						output_dict[cookie] = []
					#agora adiciona o registro de log no dicionario de saida associado com o userid dele
					output_dict[cookie].append(entry)
			#regex utilizada para retirar a data de dentro do registro de log
			regex = re.compile('(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
			#ordena todas as listas presentes no dicionario
			for key in output_dict.keys():
				#ordena a lista pela data que existe dentro dela
				output_dict[key].sort( key=lambda x: time.strptime(regex.search(x).group('time'), '%d/%b/%Y:%H:%M:%S'))
			#o chunk foi processado e os registro de logs foram agrupados por userid. Passo o dicionacirio para frente
			#TODO - verificar questão do block. Se por algum motivo a fila estiver sempre cheia, vai tratar a thread
			self.output.put(output_dict)
			logger.debug('%s','Chunk processado', extra={'type':'PROCESSOR'})

#Thrad que vai pegar os registros de log agrupados por userid e vai gravar em arquivo
class OutPutWriter(threading.Thread):

	def __init__(self, input_queue, output_folder, folder_lock):
		super(OutPutWriter, self).__init__()
		#fila onde estão os registros de log agrupados por userid(cookie)
		self.input = input_queue
		#diretório onde serão gravados os arquivos dos registro de log de cada userid
		self.output = output_folder
		#lock da pasta onde eh gravado o output
		self.folder_lock = folder_lock
		#flag que indica que a thread deve morrer
		self.dead = False

	def run(self):
		logger = logging.getLogger('log')
		log_stuff = {'type': 'WRITER'}
		logger.debug('%s', 'OutPutWrite iniciado', extra=log_stuff)
		#primeiro verifica se o diretório informado realmente existe
		if not os.path.exists(self.output):
			logger.debug('Diretório ( %s ) para gravação do output não existe.', self.output, extra=log_stuff)
			#não existe... bye,bye
			return
		#regex para pegar a data e hora dentro do registro de log
		regex = re.compile('(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
		while not self.dead:
			try:
				#pega um pouco de registros para serem gravados
				input = self.input.get(True,10)
			except Queue.Empty:
				#vish, não achou nada na file. Tentar novamente
				continue
			for userid in input.keys():
				#locka o diretório de output
				self.folder_lock.acquire()
				#abre o arquivo
				with open(self.output + os.sep + userid, 'a+') as file:
				 	file.writelines(input[userid])
					logger.debug('Gravou no arquivo %s', userid, extra=log_stuff)
				#libera o diretório de output
				self.folder_lock.release()

#método que ordena os arquivos de saída da aplicação
def sort_output(dir):
	regex = re.compile('(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
	files = os.listdir(dir)
	for file in files:
		with open(dir + os.sep + file, 'r+') as log:
			lines = log.readlines()
			lines.sort( key=lambda x: time.strptime(regex.search(x).group('time'), '%d/%b/%Y:%H:%M:%S'))
			log.seek(0)
			log.writelines(lines)

#função que cria todas as thread e objectos necessário para execução da aplicação
def run_filter():

	#objetos de lock de acesso aos diretórios
	dir_lock_0 = threading.Lock()
	dir_lock_1 = threading.Lock()
	dir_lock_2 = threading.Lock()
	dir_lock_3 = threading.Lock()

	#set que guarda os arquivos já processador do diretório
	files_done_set_0 = sets.Set()
	files_done_set_1 = sets.Set()
	files_done_set_2 = sets.Set()
	files_done_set_3 = sets.Set()

	#fila de processamento dos splits
	splits_queue = Queue.Queue()
	#thread que vão dar quebrar os arquivos de log
	splitters = []
	splitters.append(SplitLogFileTask('cluster/server-0', dir_lock_0, splits_queue, files_done_set_0))
	splitters.append(SplitLogFileTask('cluster/server-0', dir_lock_0, splits_queue, files_done_set_0))

	splitters.append(SplitLogFileTask('cluster/server-1', dir_lock_1, splits_queue, files_done_set_1))
	splitters.append(SplitLogFileTask('cluster/server-1', dir_lock_1, splits_queue, files_done_set_1))

	splitters.append(SplitLogFileTask('cluster/server-2', dir_lock_2, splits_queue, files_done_set_2))
	splitters.append(SplitLogFileTask('cluster/server-2', dir_lock_2, splits_queue, files_done_set_2))

	splitters.append(SplitLogFileTask('cluster/server-3', dir_lock_3, splits_queue, files_done_set_3))
	splitters.append(SplitLogFileTask('cluster/server-3', dir_lock_3, splits_queue, files_done_set_3))

	chunk_processed = Queue.Queue()
	processors = []
	processors.append(ChunkProcessor(splits_queue,chunk_processed ))
	processors.append(ChunkProcessor(splits_queue,chunk_processed ))
	processors.append(ChunkProcessor(splits_queue,chunk_processed ))
	processors.append(ChunkProcessor(splits_queue,chunk_processed ))

	#lock para controlar acesso no diretório onde são colocados os log filtrados
	filter_folder_lock = threading.Lock();
	writers = []
	writers.append(OutPutWriter(chunk_processed,'cluster/server-0/filter', filter_folder_lock))
	writers.append(OutPutWriter(chunk_processed,'cluster/server-0/filter', filter_folder_lock))


	for splitter in splitters:
		splitter.start()
	for processor in processors:
		processor.start()
	for writer in writers:
		writer.start()

	for splitter in splitters:
		splitter.join()
	print "Splitter threads finalizadas"

	#espera todos os split forem removidos da fila
	while not splits_queue.empty():
		pass
	for processor in processors:
		processor.dead=True
	for processor in processors:
		processor.join()

	#espera que todos os splits foram processados
	while not chunk_processed.empty():
		pass
	for writer in writers:
		writer.dead=True
	for writer in writers:
		writer.join()


	print 'Fim!'

if __name__ == '__main__':
	#configura o formato do log utilizado para debug da aplicação
	FORMAT = '%(asctime)-15s %(type)s %(thread)d %(message)s'
	logging.basicConfig(filename='log',format=FORMAT, level=logging.DEBUG)

	#BUG! http://bugs.python.org/issue7980
	time.strptime('15/Aug/2013:13:54:38', '%d/%b/%Y:%H:%M:%S')

	#roda o filtro nos log
	run_filter()
	#ordena o output
	sort_output('cluster/server-0/filter')
