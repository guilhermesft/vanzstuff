CC=gcc
CCFLAGS=-Wall -Werror -Wunreachable-code

#----------- FILES ---------------
COMMOM_FILE=commom.c log.c
SERVER_FILE=$(COMMOM_FILE) server.c
CLIENT_FILE=$(COMMOM_FILE) client.c

all:
	$(CC) $(CCFLAGS) -o bin/server $(SERVER_FILE)
	$(CC) $(CCFLAGS) -o bin/client $(CLIENT_FILE)

debug:
	$(CC) $(CCFLAGS) -g -o bin/server $(SERVER_FILE)
	$(CC) $(CCFLAGS) -g -o bin/client $(CLIENT_FILE)

clean:
	rm bin/*
