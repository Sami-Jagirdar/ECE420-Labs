/**
 * Using Second Edition code from lecture to calculate max value
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "Lab3IO.h"
#include "timer.h"

int main(int argc, char* argv[]) {

    // variables to store time
    double start, end;
     
    // variables to store matrix data
    double** G; // matrix
    double* x; // solution
    int n; // number of rows
    int m; // number of columns

    // variables used in algorithm
    double max;
    double l_max;
    int kp;
    int l_kp;
    register double l_temp; // modified frequently, may improve slightly to use register
    double* temp_row;
    int p; // number of threads

    // obtain input argument
    if (argc != 2) {
        printf("INCORRECT USAGE: Program should be run as `./datagen <p>`, where <p> is the integer max number of threads.");
        return 1;
    }

    p = atoi(argv[1]);
    if (p < 1) {
        printf("INCORRECT USAGE: The number of threads given (p) must be at least 1.\n");
        return 1;
    }

    // load input
    Lab3LoadInput(&G, &n);
    m = n + 1;
    x = CreateVec(n);

    GET_TIME(start);

    #pragma omp parallel private(l_max, l_kp, l_temp) num_threads(p)
    {
        // Gaussian Elimination
        for (int k = 0; k <= n - 2; k++) {
            
            // reinitialize necessary variables
            l_max = -DBL_MAX;
            #pragma omp single
            {
                max = -DBL_MAX;
            }

            // find row kp with max value in kth column in range [k, n)
            // split up work and find local max, then assign global max to local
            #pragma omp for
            for (int p = k; p < n; p++) {
                l_temp = fabs(G[p][k]);
                if (l_temp > l_max) {
                    #pragma omp critical
                    {
                        if (l_temp > max) {
                            max = l_temp;
                            kp = p;
                        }
                    }
                }
            }

            // use single thread to perform swap
            #pragma omp single
            {
                // swap row k and row kp
                temp_row = G[k];
                G[k] = G[kp];
                G[kp] = temp_row;
            }

            // elimination. cant collapse
            #pragma omp for
            for (int i = k + 1; i < n; i++) {
                l_temp = G[i][k] / G[k][k];
                for (int j = k; j < m; j++) {
                    G[i][j] = G[i][j] - l_temp * G[k][j];
                }
            }
        }

        // Jordan Elimination
        for (int k = n - 1; k > 0; k--) {
            #pragma omp for
            for (int i = 0; i < k; i++) {
                G[i][n] = G[i][n] - (G[i][k] / G[k][k] * G[k][n]);
                G[i][k] = 0;
            }
        }

        // Obtain solution
        #pragma omp for
        for (int i = 0; i < n; i++) {
            x[i] = G[i][n] / G[i][i];
        }
    } // end parallel

    GET_TIME(end);

    Lab3SaveOutput(x, n, end - start);
    
    // free dynamically allocated memory
    free(G);
    free(x);
}