#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "Lab3IO.h"
#include "timer.h"

int main() {
     
    // load input
    double** G;
    int n;
    Lab3LoadInput(&G, &n);
    int m = n + 1; // number of columns

    printf("=== ARRAY ===\n");
    PrintMat(G, n, m);

    // Gaussian Elimination
    for (int k = 0; k <= n - 2; k++) { // strange indexing deliberate

        printf("=== ITER %d ===\n", k);

        // find row k_p with max value in kth column in range [k, n)
        int k_p;
        double temp;
        double max_val = -DBL_MAX; // lowest possible value
        for (int p = k; p < n; p++) {
            temp = fabs(G[p][k]);
            if (temp > max_val) {
                max_val = temp;
                k_p = p;
            }
        }

        // swap row k and row k_p
        for (int i = 0; i < m; i++) {
            temp = G[k][i];
            G[k][i] = G[k_p][i];
            G[k_p][i] = temp;
        }

        printf("=== SWAP ===\n");
        PrintMat(G, n, m);

        // elimination
        for (int i = k + 1; i < n; i++) {
            temp = G[i][k] / G[k][k];
            for (int j = k; j < m; j++) {
                G[i][j] = G[i][j] - temp * G[k][j];
            }
        }

        printf("=== ELIMINATE ===\n");
        PrintMat(G, n, m);
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

    printf("=== AFTER JORDAN ELIMINATION ===\n");
    PrintMat(G, n, m);

    // Obtain solution
    double* x = CreateVec(n);
    for (int i = 0; i < n; i++) {
        x[i] = G[i][n] / G[i][i];
    }

    printf("=== SOLUTION ===\n");
    PrintVec(x, n);

    Lab3SaveOutput(x, n, 0);
}