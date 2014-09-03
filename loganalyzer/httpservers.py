#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
Esse arquivo de fonte possui a implementação dos simuladores de HTTP server.

Para simular os servidores são criados 4 processos. A única coisa que eles fazem eh
executar a função server_function. Essa função eh responsável em criar diversos
registros de log fakes.

Antes de começar a simulação eh criado todos os diretórios para execução da aplicação
'''

from multiprocessing import Process, Value
import os
import tempfile
import logging
import time
import datetime

#função que eh execudada pelos processor que simular os servidores HTTP
def server_function(dir, entries, dead):
	logger = logging.getLogger('log')
	logger.debug('%s', "Servidor iniciado", extra={'type':'SERVER'})
	if not os.path.exists(dir):
		logger.debug('%s', "Diretório não encontrado", extra={'type':'SERVER'})
		return
	#pega a quantidade de arqivos de log existente atualmente
	total_log_files = len(os.listdir(dir))
	while not dead.value == 1:
		#nome final do arquivo de log
		file_name = "log-" + str(total_log_files)
		#path do diretório do log, com a barra
		path = dir + os.sep
		#abre um arquivo oculto e popula com dados fake
		with open(path +  '.' + file_name,'w') as file:
			for entry in range(entries):
				file.write(create_log_entry(entry))
				file.write('\n')
			total_log_files += 1
		#depois que o arquivo esta ok, torna ele visivel para ser processado
		os.rename(path + '.' + file_name, path + file_name)
 		logger.debug(path + file_name, extra={'type':'SERVIDOR'})

#método que popula um registro de log fake
def create_log_entry(entry_count):
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

#Cria os diretórios para simular os servidores
def create_dir( server_count=4):
	for server in range(server_count):
		server_name = "cluster" + os.sep + "server-" + str(server)
		if not os.path.exists(server_name):
			os.makedirs(server_name)
	filter_log_dir = 'cluster' + os.sep + 'server-0/filter'
	if not os.path.exists(filter_log_dir):
		os.makedirs(filter_log_dir)

if __name__ == '__main__':
	#configura o formato do log utilizado para debug da aplicação
	FORMAT = '%(asctime)-15s %(type)s %(thread)d %(message)s'
	logging.basicConfig(filename='log-server',format=FORMAT, level=logging.DEBUG)
	#total de servidores simulados
	total_servers = 4

	create_dir(total_servers)
	dead = Value('i', 0)
	servers = [];
	servers.append(Process(target=server_function, args=('cluster/server-0', 500000, dead)))
	servers.append(Process(target=server_function, args=('cluster/server-1', 500000, dead)))
	servers.append(Process(target=server_function, args=('cluster/server-2', 500000, dead)))
	servers.append(Process(target=server_function, args=('cluster/server-3', 500000, dead)))
	for server in range(total_servers):
		servers[server].start()
	time.sleep(900)
	dead.value = 1
	for server in range(total_servers):
		servers[server].join()

	logger = logging.getLogger('log')
	logger.debug('%s', "Todos os servidores forem finalizados", extra={'type':'SERVER'})
