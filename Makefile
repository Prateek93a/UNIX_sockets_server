CC = gcc

CFLAGS = -Wall

all: server1.o server2.o server3.o client.o

server1: server1.c utility.h
	$(CC) $(CFLAGS) server1.c utility.c -o server1.o

server2: server2.c utility.h
	$(CC) $(CFLAGS) server2.c utility.c -o server2.o

server3: server3.c utility.h
	$(CC) $(CFLAGS) server3.c utility.c -o server3.o

client: client.c utility.h
	$(CC) $(CFLAGS) client.c utility.c -o client.o

utility: utility.c
	$(CC) $(CFLAGS) utility.c -o utility.o

clean:
	rm -rf *.o*