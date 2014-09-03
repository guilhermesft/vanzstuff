#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
Para tentar simplificar a execução toda a aplicação e

 Toda a aplicação foi desenvolvida usando threads. Onde cada thread executa tem uma terefa especifica. São elas:

 * SplitLogFileTask: Thread que fica observando um diretório em busca de novos arquivos de log. Quando um novo arquivo eh encontrado
 ele eh quebrado em arquivos menores e colocar em um pasta temporária.

 * ChunkProcessor: Thrad que processa os split do arquivo de log gerados e gravados na pasta temporária de split. A partir do arquivo de split
 eh gerado um dicionário onde a chave eh o valor do userid e o valor eh uma lista dos registro de log para o respectivo userid ordenado pela data e hora.
 Os dicionários gerados são colocados em um segunda fila para processamento.

 * OutPutWriter: Essa thread corresponde a parte final do processamento. Ela pega os dicionarios gerados na etapa anterior, e grava os registros de log
 em arquivo.

 Depois que todas as thread terminarem e executada uma função ( sort_output ) que ira ordenar os registro dentro do arquivo
'''

import threading
import os
import re
import datetime
import time
import sets
import Queue
import tempfile
import glob

#Thread que pega um arquivo de log e quebra em chunk menores para processamento
class SplitLogFileTask(threading.Thread):

	def __init__(self, dir, dir_lock, splitdir, splitdir_lock, files_done):
		super(SplitLogFileTask, self).__init__()
		#diretório que a thread monitora
		self.dir = dir
		#lock de acesso para o diretório
		self.dir_lock = dir_lock
		#arquivo onde será colocados os chunks
		self.splitdir = splitdir
		#lock de acesso ao diretório de split
		self.splitdir_lock = splitdir_lock
		#conjunto que contem todos os arquivos que já forem processados
		self.files_done = files_done
		#flag que indica se a thread deve morrer
		self.dead = False

	def  run(self):
		#se o arquivo de log especificado não existe, não faz nada. =(
		if not os.path.exists(self.dir):
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
				continue
			#pega um arquivo para processar
			log_file = all_log_files.pop()
			#adiciona o arquivo no conjunto de processados. Assim outras thread não vão mais pega ele
			self.files_done.add(log_file)
			#libera o diretório
			self.dir_lock.release()
			with open(log_file, 'r') as file:
				splitfile = None
				splitentries = 0
				for entry in file:
					if splitfile is None:
						self.splitdir_lock.acquire()
						splitfile =  tempfile.NamedTemporaryFile(mode='w+', dir=self.splitdir, delete=False)
						self.splitdir_lock.release()
						splitentries = 0
					#adiciona o regritro de log lido no chunk que esta sendo gerado
					splitfile.write(entry)
					splitentries += 1
					#verifica se o chunk não esta cheio
					if splitentries == 1000:
						#o chunk esta cheio, agora coloca ele na fila para processamento
						#como a queue eh thread safe não é necessário nenhum mecanismo que sincronia extra
						splitfile.close()
						#limpa o chunk para a próxima interação
						splitfile = None
				if splitfile is not None:
					splitfile.close()

#Thread que pega os chunk separados pela etapa anterior e os separa por userid
class ChunkProcessor(threading.Thread):

	def __init__(self, splitdir, splitdir_lock, output_queue, sort_key_func):
		super(ChunkProcessor, self).__init__()
		#fila onde estão os chunk pendentes
		self.splitdir = splitdir
		self.splitdir_lock = splitdir_lock
		#fila onde eh colocado o resultado do processamento
		self.output = output_queue
		#flag que indica se a thread deve morrer
	 	self. dead = False
		self.sort_key_func = sort_key_func

	def run(self):
		#enquando ninguém matar a thread... toca o barco.
		while not self.dead:
			#pega o acesso para o dir da splits
			self.splitdir_lock.acquire()
			#pegas o nome dos arquivos de split
			splits = os.listdir(self.splitdir)
			#verifica se existe algum split
			if not len(splits) > 0:
				self.splitdir_lock.release()
				#não tem split... e vai novamente
				continue
			split_entries = None
			lf = self.splitdir + os.sep + splits[0]
			with open(lf) as splitfile:
				#carrega o split para memória
				split_entries = splitfile.readlines()
			os.remove(splitfile.name)
			#libera a pasta de splits
			self.splitdir_lock.release()
			if split_entries is not None:
           			regex = re.compile(r'"userid=(?P<biscoito>.+)"')
				output_dict = {}
				for entry in split_entries:
					#procura pelo cookie no registro do log
					result = regex.search(entry)
					if result is None:
						#algum registro zuado, a regez não consegui pegar o cookie
						continue
					cookie = result.group('biscoito')
					if cookie is not None:
						#verifica se esse cookie jah não foi encontrado no chunk processado
						if not output_dict.has_key(cookie):
							#esse userid ainda não foi processado, então precisamos colocar ele no dicionario
							output_dict[cookie] = []
						#agora adiciona o registro de log no dicionario de saida associado com o userid dele
						output_dict[cookie].append(entry)
				for key in output_dict.keys():
					#ordena a lista pela data que existe dentro dela
					output_dict[key].sort( key= lambda x: self.sort_key_func(x))
				#o chunk foi processado e os registro de logs foram agrupados por userid. Passo o dicionacirio para frente
				self.output.put(output_dict)


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
		log_stuff = {'type': 'WRITER'}
		#primeiro verifica se o diretório informado realmente existe
		if not os.path.exists(self.output):
			#não existe... bye,bye
			return
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
				#libera o diretório de output
				self.folder_lock.release()

#função passada para a função de sort para pegar o valor que deve ser comparado na ordenação
def sort_key(x):
	if not x.strip():
		try:
			regex_time = re.compile(r'(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
			result = regex_time.search(x)
			if result is not None:
				strtime = result.group('time')
				return time.strptime(strtime, '%d/%b/%Y:%H:%M:%S')
		except Exception as e:
			pass
	return 0

#método que ordena os arquivos de saída da aplicação
def sort_output(dir):
	regex = re.compile(r'(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
	files = os.listdir(dir)
	for file in files:
		with open(dir + os.sep + file, 'r+') as log:
			lines = log.readlines()
			lines.sort( key=lambda x:sort_key(x))
			log.seek(0)
			log.writelines(lines)

#função que cria todas as thread e objectos necessário para execução da aplicação
def run_filter():

	#objetos de lock de acesso aos diretórios
	dir_lock_0 = threading.Lock()
	dir_lock_1 = threading.Lock()
	dir_lock_2 = threading.Lock()
	dir_lock_3 = threading.Lock()
	splitdir_lock = threading.Lock()

	#set que guarda os arquivos já processador do diretório
	files_done_set_0 = sets.Set()
	files_done_set_1 = sets.Set()
	files_done_set_2 = sets.Set()
	files_done_set_3 = sets.Set()

	#cria a pasta temporaria para colocar os arquivos splitados
	splitsdir = '.splits'
	if not os.path.exists(splitsdir):
		os.makedirs(splitsdir)

	#thread que vão dar quebrar os arquivos de log
	splitters = []
	splitters.append(SplitLogFileTask('cluster/server-0', dir_lock_0, splitsdir, splitdir_lock, files_done_set_0))
	splitters.append(SplitLogFileTask('cluster/server-0', dir_lock_0, splitsdir, splitdir_lock, files_done_set_0))
	splitters.append(SplitLogFileTask('cluster/server-1', dir_lock_1, splitsdir, splitdir_lock, files_done_set_1))
	splitters.append(SplitLogFileTask('cluster/server-1', dir_lock_1, splitsdir, splitdir_lock, files_done_set_1))
	splitters.append(SplitLogFileTask('cluster/server-2', dir_lock_2, splitsdir, splitdir_lock, files_done_set_2))
	splitters.append(SplitLogFileTask('cluster/server-2', dir_lock_2, splitsdir, splitdir_lock, files_done_set_2))
	splitters.append(SplitLogFileTask('cluster/server-3', dir_lock_3, splitsdir, splitdir_lock, files_done_set_3))
	splitters.append(SplitLogFileTask('cluster/server-3', dir_lock_3, splitsdir, splitdir_lock, files_done_set_3))
	chunk_processed = Queue.Queue()
	processors = []
 	processors.append(ChunkProcessor(splitsdir, splitdir_lock,chunk_processed, sort_key ))
	processors.append(ChunkProcessor(splitsdir, splitdir_lock,chunk_processed, sort_key ))
	processors.append(ChunkProcessor(splitsdir, splitdir_lock,chunk_processed, sort_key ))
	processors.append(ChunkProcessor(splitsdir, splitdir_lock,chunk_processed, sort_key ))

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

	while len(os.listdir(splitsdir)) > 0:
		pass
	for splitter in splitters:
		splitter.join()
	print "Splitter threads finalizadas"

	for processor in processors:
		processor.dead=True
	for processor in processors:
		processor.join()
	print 'Processor threads finalizadas'

	#espera que todos os splits foram processados
	while not chunk_processed.empty():
		pass
	for writer in writers:
		writer.dead=True
	for writer in writers:
		writer.join()
	print 'Writer thread finalizadas'

if __name__ == '__main__':
	#BUG! http://bugs.python.org/issue7980
	time.strptime('15/Aug/2013:13:54:38', '%d/%b/%Y:%H:%M:%S')

	#roda o filtro nos log
	run_filter()
	#ordena o output
	sort_output('cluster/server-0/filter')
	print 'Fim! =]'
