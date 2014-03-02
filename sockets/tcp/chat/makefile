CC=gcc
CCFLAGS=-Wall -Werror -Wunreachable-code

all:
	$(CC) $(CCFLAGS) -o server server.c
	$(CC) $(CCFLAGS) -o client client.c

clean:
	rm client server
