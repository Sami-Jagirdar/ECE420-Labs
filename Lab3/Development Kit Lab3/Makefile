CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = main
SRC = main.c

all: $(TARGET)

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o main

main_seq: main_seq.c
	$(CC) $(CFLAGS) main_seq.c Lab3IO.c -o main_seq

datagen: 
	$(CC) datagen.c Lab3IO.c -o datagen

clean:
	rm -f $(TARGET)