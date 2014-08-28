#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
 Toda a aplicação foi desenvolvida usando threads. Onde cada thread executa tem uma terefa especifica. São elas:
 * HTTPServer: Thread que simular um servidor HTTP constantemente criando arquivos de log.
 * SplitLogFileTask: Thread que fica observando um diretório em busca de novos arquivos de log. Entado um arquivos eh
                     feito um split dele em chunk menores. Esses chunk são colocados em um fila para processamento futuro
 * ChunkProcessor: Thrad que processa os chunk gerados. Na parte de processamento, eh gerado um dicionário onde a chave eh
 		   o valor do userid e o valor eh uma lista dos registro de log para o respectivo userid ordenado pela data e hora.
		   Os dicionários gerados são colocados em um segunda fila para processamento.
 * SaveFilterEntries: Essa thread corresponde a parte final do processamento. Ela é responsável em gravar os resgistros separados e
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
import shutil

#Thread que simula um servidor HTTP lançando logs
class HTTPServer(threading.Thread):

	def __init__(self, server_dir, entries_log_file=1000):
		super(HTTPServer, self).__init__()
		#diretório onde o servidor ira gravar os arquivos de log
		self.dir = server_dir
		#total de registros de log em cada arquivo de log
		self.entries = entries_log_file
		#flag que indica que a thread deve morrer
	 	self.dead = False

	def run(self):
		logger = logging.getLogger('log')
		logger.debug('%s', "Servidor iniciado", extra={'type':'SERVER'})
		if not os.path.exists(self.dir):
			logger.debug('%s', "Diretório não encontrado", extra={'type':'SERVER'})
			return
		#pega a quantidade de arqivos de log existente atualmente
		total_log_files = len(os.listdir(self.dir))
		log_entry_id = 1;
		while not self.dead:
			with tempfile.NamedTemporaryFile(delete=False) as file:
				for entry in range(self.entries):
					file.write(self.create_log_entry(log_entry_id))
					file.write('\n')
					log_entry_id += 1
					time.sleep(0.5)
				shutil.move(file.name, self.dir + os.sep + "log-" + str(total_log_files))
				total_log_files += 1
				logger.debug("Novo arquivo de log: %s ", self.dir + os.sep + "log-" + str(total_log_files), extra={'type':'SERVIDOR'})

	#Cria um registro de log fake
	def create_log_entry(self, entry_count):
		log_entry_template = '@@ip@@ - - [@@time@@ -0300] "GET @@file@@ HTTP/1.1" 200 @@reg@@ "-" "userid=@@cookie@@"'
		#replace da data e hora
		fake_entry = log_entry_template.replace("@@time@@", datetime.datetime.fromtimestamp(time.time()).strftime('%d/%b/%Y:%H:%M:%S'))
		#replace do arquivo
		fake_entry = fake_entry.replace("@@file@@","/file" + str(entry_count))
		#replace do registro
		fake_entry = fake_entry.replace("@@reg@@", str(entry_count))
		#para teste tesmos somente 2 userid
		#primeiro fazemos replace do ip e do biscoito
		if entry_count % 2 == 0:
			fake_entry = fake_entry.replace("@@ip@@","192.168.100.001")
			fake_entry = fake_entry.replace("@@cookie@@","biscoito-do-vanz-1")
		else:
			fake_entry = fake_entry.replace("@@ip@@","192.168.100.002")
			fake_entry = fake_entry.replace("@@cookie@@","biscoito-do-vanz-2")
		return fake_entry

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
			#cria um conjunto com todos os arquivos
			all_log_files = set(os.listdir(self.dir))
			#da lista de todos os arquivos de log, retira o que jpa foi processado
			all_log_files.difference_update(self.files_done)
			#para não precisar tratar a exception, vou verificar se tem algo antes
			if len(all_log_files) == 0:
				self.dir_lock.release()
				continue
			#pega um arquivo para processar
			log_file = all_log_files.pop()
			#adiciona o arquivo no conjunto de processados. Assim outras thread não vão mais pega ele
			self.files_done.add(log_file)
			#libera o diretório
			self.dir_lock.release()
			with open(self.dir + os.sep + log_file, 'r') as file:
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
	 	self.dead = False

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
			regex = re.compile('userid=(?P<biscoito>.+)')
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
				output_dict[key].sort( key=lambda x: datetime.datetime.strptime(regex.search(x).group('time'), '%d/%b/%Y:%H:%M:%S'))
			#o chunk foi processado e os registro de logs foram agrupados por userid. Passo o dicionacirio para frente
			#TODO - verificar questão do block. Se por algum motivo a fila estiver sempre cheia, vai tratar a thread
			self.output.put(output_dict)

	def sort_function(L):
		pass

#Thrad que vai pegar os registros de log agrupados por userid e vai gravar em arquivo
class SaveFilterEntries(threading.Thread):

	def __init__(self, input_queue, output_folder):
		#fila onde estão os registros de log agrupados por userid(cookie)
		self.input = input_queue
		#diretório onde serão gravados os arquivos dos registro de log de cada userid
		self.output = output_folder
		#flag que indica que a thread deve morrer
		self.dead = false

	def run(self):
		logger = logging.getLogger('log')
		#primeiro verifica se o diretório informado realmente existe
		if not os.path.exists(self.output):
			#não existe... bye,bye
			return
		#regex para pegar a data e hora dentro do registro de log
		regex = re.compile('(?P<time>\d\d/\w{3}/\d{4}(:\d\d){3})')
		while not dead:
			#pega um pouco de registros para serem gravados. TODO Verificar se der timeout lança exception ou retorna none
			input = self.input.get(true,1000)
			for userid in input.keys():
				#abre o arquivo
				with open(self.output + os.sep + userid, 'rw+') as file:
					for entry in input[userid]:
				 		file.write(entry)

#Cria os diretórios para simular os servidores
def create_dir( server_count=4):
	for server in range(server_count):
		server_name = "cluster" + os.sep + "server-" + str(server)
		if not os.path.exists(server_name):
			os.makedirs(server_name)



#configura o formato do log utilizado para debug da aplicação
FORMAT = '%(asctime)-15s %(type)s %(thread)d %(message)s'
logging.basicConfig(filename='log',format=FORMAT, level=logging.DEBUG)

create_dir(4)
#threads que simulam servidor http
servers = []
servers.append(HTTPServer('cluster/server-0', 1000))
servers.append(HTTPServer('cluster/server-1', 1000))
servers.append(HTTPServer('cluster/server-2', 1000))
servers.append(HTTPServer('cluster/server-3', 1000))

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
splitters.append(SplitLogFileTask(servers[0].dir, dir_lock_0, splits_queue, files_done_set_0))
splitters.append(SplitLogFileTask(servers[0].dir, dir_lock_0, splits_queue, files_done_set_0))

splitters.append(SplitLogFileTask(servers[1].dir, dir_lock_1, splits_queue, files_done_set_1))
splitters.append(SplitLogFileTask(servers[1].dir, dir_lock_1, splits_queue, files_done_set_1))

splitters.append(SplitLogFileTask(servers[2].dir, dir_lock_2, splits_queue, files_done_set_2))
splitters.append(SplitLogFileTask(servers[2].dir, dir_lock_2, splits_queue, files_done_set_2))

splitters.append(SplitLogFileTask(servers[3].dir, dir_lock_3, splits_queue, files_done_set_3))
splitters.append(SplitLogFileTask(servers[3].dir, dir_lock_3, splits_queue, files_done_set_3))

chunk_processed = Queue.Queue()
processors = []
processors.append(ChunkProcessor(splits_queue,chunk_processed ))
processors.append(ChunkProcessor(splits_queue,chunk_processed ))
processors.append(ChunkProcessor(splits_queue,chunk_processed ))
processors.append(ChunkProcessor(splits_queue,chunk_processed ))

for server in servers:
	server.start()
for splitter in splitters:
	splitter.start()
for processor in processors:
	processor.start()

#para e espera as thread dos servidores finalizar
for server in servers:
	server.dead = True
for server in servers:
	server.join()
print "Servers thread finalizadas"

#para todas as thread de splitt
for splitter in splitters:
	splitter.dead = True
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
#while not chunk_processed.empty():
#	pass
print 'Fim!'
