CC=gcc
CCFLAGS= -Wall

signal_test:
	$(CC) $(CCFLAGS) -o signal_test signal_test.c

fifo:
	$(CC) $(CCFLAGS) -o fifo_productor fifo_productor.c
	$(CC) $(CCFLAGS) -o fifo_consumer fifo_consumer.c

clean:
	rm signal_test fifo_productor fifo_consumer
