CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = demo matrixgen
SRC = demo.c lab1_IO.c

all: $(TARGET)

demo: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o demo -lm -lpthread

matrixgen: 
	$(CC) matrixgen.c -o matrixgen 

# test file we used to observe speedup in parallel implementation
demo_seq: demo_seq.c
	$(CC) $(CFLAGS) demo_seq.c lab1_IO.c -o demo_seq -lm

clean:
	rm -f $(TARGET)