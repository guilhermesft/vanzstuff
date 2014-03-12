CC=gcc
CCFLAGS=-Wall -Werror -Wunreachable-code

all:
	$(CC) $(CCFLAGS) -o server commom.c server.c
	$(CC) $(CCFLAGS) -o client commom.c client.c log.c

debug:
	$(CC) $(CCFLAGS) -g -o server server.c commom.c
	$(CC) $(CCFLAGS) -g -o client client.c log.c commom.c

server:
	$(CC) $(CCFLAGS) -o server commom.c server.c


clean:
	rm client server
