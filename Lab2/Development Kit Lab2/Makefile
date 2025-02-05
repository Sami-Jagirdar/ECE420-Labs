CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = main client attacker
SRC = server.c

all: $(TARGET)

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o main -lpthread

client: 
	$(CC) client.c -o client 

attacker:
	$(CC) attacker.c -o attacker -lm

tests:
	$(CC) server-1rw.c -o server-1rw
	$(CC) server-mutex.c -o server-mutex

clean:
	rm -f $(TARGET)