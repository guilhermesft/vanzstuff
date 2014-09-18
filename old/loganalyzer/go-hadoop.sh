#!/bin/bash

#função que limpa possiveis sujeiras de build passados
function clean()
{
	if [ -d LogFilter*.class ]; then
		rm LogFilter*.class
	fi
	if [ -d logfilter.jar ]; then
		rm logfilter.jar
	fi
}

#função que cria os diretório necessários
function create_dirs()
{
	mkdir -p cluster/server-0
	mkdir -p cluster/server-1
	mkdir -p cluster/server-2
	mkdir -p cluster/server-3
}

#função que compila e roda o mapreduce job
function compile_run()
{
	#compila os fontes
	~/hadoop-2.5.0/bin/hadoop com.sun.tools.javac.Main LogFilter.java
	#gera o jar
	jar cf logfilter.jar LogFilter*.class
	#o job vai criar a pasta de output, então se ele ja existir vou apagar
	if [ -d cluster/server-0/filter ]; then
		rm -rf cluster/server-0/filter
	fi
	~/hadoop-2.5.0/bin/hadoop jar logfilter.jar LogFilter cluster/server-*/log-* cluster/server-0/filter
}

clean
create_dirs
compile_run
#não gosto dos .class no diretório, por isso vou limpar eles
rm LogFilter*.class
