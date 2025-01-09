#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lab1_IO.h"
#include "timer.h"

int main(void) {
    printf("test\n");

    double start, end;

    // load input matrices A and B
    int **A; int **B; int n;
    Lab1_loadinput(&A, &B, &n);
    printf("%d\n", n);

    // Allocate memory for output matrix C
    int **C = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
       *(C+i) = malloc(n * sizeof(int));
    }

    // perform sequential multiplication
    GET_TIME(start);

    for (int i = 0; i < n; i++ ) {
        for (int j=0; j<n; j++) {
            int result = 0;
            for (int k=0; k < n; k++) {
                result = result + A[i][k] * B[k][j];
            }
            C[i][j] = result;
        }
    }
    
    GET_TIME(end);

    // create output file
    double time = end - start;
    printf("Process completed in %lf\n", time);
    Lab1_saveoutput(C, &n, time);
}