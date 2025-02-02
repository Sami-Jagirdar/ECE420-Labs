CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = main client
SRC = server.c

all: $(TARGET)

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o main -lm -lpthread

client: 
	$(CC) client.c -o client 

clean:
	rm -f $(TARGET) client