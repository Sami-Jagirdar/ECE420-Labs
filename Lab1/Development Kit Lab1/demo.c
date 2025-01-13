#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lab1_IO.h"
#include "timer.h"

// global variables
int **A; int **B; int **C; // store matrices
int n; int p;              // store important paramers
int root_p;                // small optimization to avoid unnecessary computation

void* threadfunc (void* k) {
    // obtain x and y from formulas in lab manual
    int x = floor(*(int *)(k) / root_p);
    int y = *(int *)k % (int) root_p;
    int result = 0;

    // keep these values in processor registers for optimization, since they are modified very frequently
    register int i = 0, j = 0, z = 0;

    for (i = (n * x) / root_p ; i <= n * (x+1) / root_p - 1 ; i++) {
        for (j = (n * y) / root_p ; j <= n * (y+1) / root_p - 1 ; j++) {
            result = 0;
            for (z = 0 ; z < n ; z++) {
                result = result + A[i][z] * B[z][j];
            }
            C[i][j] = result;
        }
    }
    return NULL;
};


int main(int argc, char **argv) {

    double start, end;

    // get command line argument
    if (argc != 2) {
        printf("INCORRECT INPUT: This program must be run with one argument (the number of threads, p).\n");
        return 1;
    }
    
    p = atoi(argv[1]);
    if (p < 1) {
        printf("INCORRECT INPUT: The number of threads given (p) must be at least 1.\n");
        return 1;
    }

    // load input matrices A and B
    Lab1_loadinput(&A, &B, &n);

    // perform input validation given lab manual constraints
    if ((n * n) % p != 0) {
        printf("INCORRECT INPUT: The number of threads given (p) must perfectly divide the square of the side length of the input matrices (n * n).\n");
        return 1;
    } else if (sqrt(p) != (int) sqrt(p)) {
        printf("INCORRECT INPUT: The number of threads given (p) must be a perfect square number.\n");
        return 1;
    }
    root_p = sqrt(p); // small optimization to avoid computing within threads

    printf("Program run with p = %d, n = %d.\n", p, n);

    // allocate memory for output matrix C
    C = malloc(n * sizeof(int*));
    for (int i = 0 ; i < n ; i++) {
       C[i] = malloc(n * sizeof(int));
    }

    // allocate memory for thread handles and arguments
    pthread_t *thread_handles = malloc(p * sizeof(pthread_t));
    int *ranks = malloc(p * sizeof(int)); // array of ranks to pass to threads
    for (int r = 0 ; r < p ; r++) {
        ranks[r] = r;
    }

    // initialize threads and perform multiplication
    GET_TIME(start);
    for (int r = 0 ; r < p ; r++) {
        pthread_create(&thread_handles[r], NULL, threadfunc, (void *) &ranks[r]);
    }

    // collect threads
    for (int r = 0 ; r < p ; r++) {
        pthread_join(thread_handles[r], NULL);
    }
    GET_TIME(end);

    // create output file
    double time = end - start;
    printf("Process completed in %lf\n", time);
    Lab1_saveoutput(C, &n, time);
    
    // release alllocated memory
    free(A); free(B); free(C);
    free(thread_handles);
    free(ranks);
}