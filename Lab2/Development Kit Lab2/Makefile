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

clean:
	rm -f $(TARGET)