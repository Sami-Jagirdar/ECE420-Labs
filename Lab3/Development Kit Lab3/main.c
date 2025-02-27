#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "Lab3IO.h"
#include "timer.h"

int main() {

    double start, end;
     
    // load input
    double** G;
    int n;
    Lab3LoadInput(&G, &n);
    int m = n + 1; // number of columns

    printf("=== ARRAY ===\n");
    PrintMat(G, n, m);

    // Variables
    double max;
    double l_max;
    double l_temp;
    int kp;
    int l_kp;

    // Gaussian Elimination
    GET_TIME(start);

    #pragma omp parallel private(l_max, l_kp, l_temp)
    {

    for (int k = 0; k <= n - 2; k++) { // strange indexing deliberate
        
        // reinitialize necessary variables
        #pragma omp single
        {
            max = -DBL_MAX;
        }
        l_max = -DBL_MAX;

        printf("=== ITER %d ===\n", k);

        // find row kp with max value in kth column in range [k, n)
        // split up work and find local max, then assign global max to local
        #pragma omp for
        for (int p = k; p < n; p++) {
            l_temp = fabs(G[p][k]);
            if (l_temp > l_max) {
                l_max = l_temp;
                l_kp = p;
            }
        }
        if (l_max > max) {
            #pragma omp critical
            {
                if (l_max > max) {
                    max = l_max;
                    kp = l_kp;
                }
            }
        }
        // need barrier after crit section to ensure max of whole column actually found
        #pragma omp barrier

        // use single thread to perform swap
        #pragma omp single
        {
            // swap row k and row kp
            double* temp_row;
            temp_row = G[k];
            G[k] = G[kp];
            G[kp] = temp_row;

            printf("=== SWAP kp=%d ===\n", kp);
            PrintMat(G, n, m);
        }

        // elimination
        // shoul i collapse for loop?
        #pragma omp for
        for (int i = k + 1; i < n; i++) {
            l_temp = G[i][k] / G[k][k];
            for (int j = k; j < m; j++) {
                G[i][j] = G[i][j] - l_temp * G[k][j];
            }
        }

        #pragma omp single
        {
            printf("=== ELIMINATE ===\n");
            PrintMat(G, n, m);
        }
    }

    #pragma omp single
    {
        printf("=== AFTER GAUSSIAN ELIMINATION ===\n");
        PrintMat(G, n, m);
    }

    // Jordan Elimination

    #pragma omp for
    for (int k = n - 1; k > 0; k--) {
        for (int i = 0; i < k; i++) {
            G[i][n] = G[i][n] - (G[i][k] / G[k][k] * G[k][n]);
            G[i][k] = 0;
        }
    }

    }

    GET_TIME(end);

    printf("=== AFTER JORDAN ELIMINATION ===\n");
    PrintMat(G, n, m);

    // Obtain solution
    double* x = CreateVec(n);
    for (int i = 0; i < n; i++) {
        x[i] = G[i][n] / G[i][i];
    }

    printf("=== SOLUTION ===\n");
    PrintVec(x, n);
    printf("TIME: %f\n", end - start);

    Lab3SaveOutput(x, n, end - start);

    free(G);
    free(x);
}