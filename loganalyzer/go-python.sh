#!/bin/bash

#Script para rodar o filtro de log implementado em pyhon

#função que limpa possiveis sujeiras de execuções
function clean
{
	#limpa o diretório de arquivos temporarios da execução
	if [ -d .splits ]; then
		rm -rf .splits
	fi
	#apaga o log de execução
	if [ -d log ];then
		rm log
	fi
	#apaga resultado de execuções passadas
	rm -rf cluster/server-0/filter/*
}

#função que roda o filtro de log
function run
{
	python log_filter_job.py
}

clean
run
