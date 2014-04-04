#!/bin/bash

START_SERVER=false
START_CLIENT=false
DEBUG=

function show_help(){
	echo "--server : Init server"
	echo "--client : Init client"
}

while [ "$1" ]; do
	case "$1" in
		"--server")
			START_SERVER=true
		;;
		"--client")
			START_CLIENT=true
		;;
		"--debug")
			DEBUG=gdb
		;;
		"--help")
			show_help
		;;
		*)
			echo "Unknown command: $1. See --help to list possibles commands." >&2
			exit 1
		;;
	esac
	shift
done

if [ $START_SERVER ]; then
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib $DEBUG ./bin/server
fi

if [ $START_CLIENT ]; then
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib $DEBUG ./bin/client
fi
