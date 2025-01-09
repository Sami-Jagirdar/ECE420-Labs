CC = gcc
CFLAGS = -Wall -Wextra
TARGET = demo
SRC = demo.c lab1_IO.c

all: $(TARGET)

demo: $(SRC)
	$(CC) $(CFLAGS) -o demo $(SRC)

matrixgen: 
	gcc matrixgen.c -o matrixgen

clean:
	rm -f $(TARGET)