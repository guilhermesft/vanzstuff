CC=gcc
CFLAGS= -Wall -Werror
LIBS=-lrt

all:
	$(CC) $(CFLAGS) -o shared main.c $(LIBS)

clean:
	rm shared
	rm /dev/shm/myshm
