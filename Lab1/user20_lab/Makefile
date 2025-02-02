CC = gcc
CFLAGS = -g -Wall -Wextra
TARGET = main
SRC = main.c lab1_IO.c

all: $(TARGET)

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o main -lm -lpthread

# matrixgen: 
# 	$(CC) matrixgen.c -o matrixgen 

# test file we used to observe speedup in parallel implementation
# demo_seq: demo_seq.c
# 	$(CC) $(CFLAGS) demo_seq.c lab1_IO.c -o demo_seq -lm

# clean:
# 	rm -f $(TARGET) demo_seq data_input data_output