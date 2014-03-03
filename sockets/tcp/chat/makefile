CC=gcc
CCFLAGS=-Wall -Werror -Wunreachable-code

all:
	$(CC) $(CCFLAGS) -o server server.c
	$(CC) $(CCFLAGS) -o client client.c log.c

debug:
	$(CC) $(CCFLAGS) -g -o server server.c
	$(CC) $(CCFLAGS) -g -o client client.c log.c


clean:
	rm client server
