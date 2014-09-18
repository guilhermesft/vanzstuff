CC=gcc
CCFLAGS=-Wall -Werror -Wunreachable-code

#----------- FILES ---------------
COMMOM_FILE=src/commom.c src/log.c
SERVER_FILE=$(COMMOM_FILE) src/server.c
CLIENT_FILE=$(COMMOM_FILE) src/client.c
INCLUDE=-Iinclude -I3rd_party
LIB_DIR=-Llib
LIBS =-ltanaguara

all:
	$(CC) $(CCFLAGS) $(INCLUDE) $(LIB_DIR) -o bin/server $(SERVER_FILE) $(LIBS)
	$(CC) $(CCFLAGS) $(INCLUDE) $(LIB_DIR) -o bin/client $(CLIENT_FILE) $(LIBS)

debug:
	$(CC) $(CCFLAGS) $(INCLUDE) -g $(LIB_DIR) -o bin/server $(SERVER_FILE) $(LIBS)
	$(CC) $(CCFLAGS) $(INCLUDE) -g $(LIB_DIR) -o bin/client $(CLIENT_FILE) $(LIBS)

clean:
	rm bin/*
