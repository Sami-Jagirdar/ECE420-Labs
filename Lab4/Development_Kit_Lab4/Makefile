CFLAGS = -g -Wall -Wextra

all: main

main: main.c Lab4_IO.c
	mpicc $(CFLAGS) main.c Lab4_IO.c -o main -lm

main_seq: main_seq.c Lab4_IO.c
	gcc $(CFLAGS) main_seq.c Lab4_IO.c -o main_seq -lm

datatrim: datatrim.c
	gcc datatrim.c -o datatrim

test:
	mpicc $(CFLAGS) main_test3.c Lab4_IO.c -o main -lm

clean:
	rm -f main main_seq datatrim