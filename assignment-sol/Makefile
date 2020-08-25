CC = gcc

CFLAGS = -Wall

all: server1.o server2.o server3.o client.o

server1: utility.o
	$(CC) $(CFLAGS) server1.c utility.c -o server1.o

server2: utility.o
	$(CC) $(CFLAGS) server2.c utility.c -o server2.o

server3: utility.o
	$(CC) $(CFLAGS) server3.c utility.c -o server3.o

utility: 
	$(CC) $(CFLAGS) utility.c -o utility.o

client:
	$(CC) $(CFLAGS) client.c -o client.o

clean:
	rm -rf *.o*