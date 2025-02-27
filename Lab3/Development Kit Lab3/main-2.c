#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <omp.h>
#include "Lab3IO.h"
#include "timer.h"

int main() {
    double start, end;
     
    // load input
    double** G;
    int n;
    Lab3LoadInput(&G, &n);
    int m = n + 1; // number of columns
    PrintMat(G, n, m);

    GET_TIME(start);

    // globals
    int kp;
    double max = -DBL_MAX; // lowest possible value
    
    // local variables for each thread
    int l_kp;
    double l_temp;
    double l_max = -DBL_MAX;
    
    #pragma omp parallel private(l_kp, l_temp) firstprivate(l_max)
    {

    // Gaussian Elimination
    for (int k = 0; k <= n - 2; k++) { // strange indexing deliberate

        // find local maximum
        #pragma omp for
        for (int p = k; p < n; p++) {
            l_temp = fabs(G[p][k]);
            if (l_temp > l_max) {
                // save max value and corresponding column
                l_max = l_temp;
                l_kp = p;
            }
        }
        // compare with global maximum
        if (l_max > max) {
            #pragma omp critical
            {
                if (l_max > max) {
                    max = l_max;
                    kp = l_kp;
                }
            }
        }
        #pragma omp barrier
        
        #pragma omp single
        {

            // find row kp with max value in kth column in range [k, n)
            // int kp;
            // double temp;
            // double max_val = -DBL_MAX; // lowest possible value
            // for (int p = k; p < n; p++) {
            //     temp = fabs(G[p][k]);
            //     if (temp > max_val) {
            //         max_val = temp;
            //         kp = p;
            //     }
            // }

            // swap row k and row kp
            double* temp_row;
            temp_row = G[k];
            G[k] = G[kp];
            G[kp] = temp_row;

            printf("=== SWAP kp=%d ===\n", kp);
            PrintMat(G, n, m);

            // elimination
            for (int i = k + 1; i < n; i++) {
                l_temp = G[i][k] / G[k][k];
                for (int j = k; j < m; j++) {
                    G[i][j] = G[i][j] - l_temp * G[k][j];
                }
            }

            printf("=== ELIMINATE ===\n");
            PrintMat(G, n, m);
        }
    }

    }

    printf("=== AFTER GAUSSIAN ELIMINATION ===\n");
    PrintMat(G, n, m);

    // Jordan Elimination

    for (int k = n - 1; k > 0; k--) {
        for (int i = 0; i < k; i++) {
            printf("i=%d, k=%d\n", i, k);
            G[i][n] = G[i][n] - (G[i][k] / G[k][k] * G[k][n]);
            printf("%f\n", G[i][n]);
            G[i][k] = 0;
        }

        printf("=== ELIMINATE k=%d ===\n", k);
        PrintMat(G, n, m);
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