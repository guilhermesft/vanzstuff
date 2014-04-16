CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lrt

posix:
	$(CC) $(CFLAGS) -o posix shmposix.c $(LIBS)
sysv:
	$(CC) $(CFLAGS) -o sysv shmsysv.c $(LIBS)


clean:
	rm posix
	rm sysv
