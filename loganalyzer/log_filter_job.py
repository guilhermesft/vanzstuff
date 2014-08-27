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
import queue

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
		#pega a quantidade de arqivos de log existente atualmente
		total_log_files = len(os.listdir(self.dir))
		log_entry_id = 1;
		while not self.dead:
			with open(self.dir + os.sep + "log-" + str(total_log_files), 'w') as file:
				for entry in range(self.entries):
					file.write(self.create_log_entry(log_entry_id))
					file.write('\n')
					log_entry_id += 1
				total_log_files += 1

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

	def __init__(self, log_file, input_queue):
		self.log_file = log_file
		self.output_queue = input_queue

	def run(self):
		#se o arquivo de log especificado não existe, não faz nada. =(
		if not os.path.exists(self.log_file):
			return
		#o arquivo existe, agora vamos ler e gerar os chunk para processamento
		#para cada 100 registros encontrados nos arquivos sera gerado um outro chunck
		with open(self.log_file, 'rw') as file:
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
 			#TODO - talvez fazer um controle de todos os arquivos de log lidos

#Thread que pega os chunk separados pela etapa anterior e os separa por userid
class ChunkProcessor(threading.Thread):

	def __init__(self, input_queue, output_queue):
		#fila onde estão os chunk pendentes
		self.input = input_queue
		#fila onde eh colocado o resultado do processamento
		self.output = output_queue
		#flag que indica se a thread deve morrer
		self.dead = false

	def run(self):
		#enquando ninguém matar a thread... toca o barco.
		while not dead:
			#tenta pegar um chunk na fila de chunk pendentes. TODO - verificar se der timeout lança exception ou só retorna none
			chunk = self.input.get(true, 1000)
			#expressão regular utilizada para pegar o valor do biscoito do registro de log
			regex = re.compile('userid=(?P<biscoito>.+)')
			output_dict = {}
			for entry in chunk:
				#procura pelo cookie no registro do log
				cookie = regex.match(entry).group(0)
				if cookie is not None:
					#verifica se esse cookie jah não foi encontrado no chunk processado
					if not output_dict.has_key(cookie):
						#esse userid ainda não foi processado, então precisamos colocar ele no dicionario
						output_dict[cookie] = []
					#agora adiciona o registro de log no dicionario de saida associado com o userid dele
					output_dict[cookie].append(entry)
			#ordena todas as listas presentes no dicionario
			for key in output_dict.keys():
				#TODO - ordenar por data e hora
				output_dict[key].sort()
			#o chunk foi processado e os registro de logs foram agrupados por userid. Passo o dicionacirio para frente
			#TODO - verificar questão do timeout
			self.output.put(output_dict)

	def sort_function():
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


create_dir(4)
#threads que simulam servidor http
servers = []
servers.append(HTTPServer('cluster/server-0'))
servers.append(HTTPServer('cluster/server-1'))
servers.append(HTTPServer('cluster/server-2'))
servers.append(HTTPServer('cluster/server-3'))
#thread que vão fazer o split

split_queue = Queue(100)



for server in servers:
	server.start()
time.sleep(180)
for server in servers:
	server.dead = True
for server in servers:
	server.join()

print 'Fim!'
