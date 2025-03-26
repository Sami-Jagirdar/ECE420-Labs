CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = main
SRC = main.c Lab3IO.c

all: $(TARGET)

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o main -fopenmp

main_seq: main_seq.c
	$(CC) $(CFLAGS) main_seq.c Lab3IO.c -o main_seq

main_test1: main_test1.c
	$(CC) $(CFLAGS) main_test1.c Lab3IO.c -o main_test1 -fopenmp

main_test2: main_test2.c
	$(CC) $(CFLAGS) main_test2.c Lab3IO.c -o main_test2 -fopenmp

main_test3: main_test3.c
	$(CC) $(CFLAGS) main_test3.c Lab3IO.c -o main_test3 -fopenmp

main_test4: main_test4.c
	$(CC) $(CFLAGS) main_test4.c Lab3IO.c -o main_test4 -fopenmp

datagen: 
	$(CC) datagen.c Lab3IO.c -o datagen

clean:
	rm -f $(TARGET)