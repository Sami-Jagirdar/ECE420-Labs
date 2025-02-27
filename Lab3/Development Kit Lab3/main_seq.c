#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "Lab3IO.h"
#include "timer.h"

int main() {
     
    // load input
    double** G;
    int n;
    Lab3LoadInput(&G, &n);

    // Gaussian Elimination
    for (int k = 0; k < n; k++) {

        // find row k_p with max value in kth column
        int k_p;
        double max_val = -DBL_MAX; // lowest possible value
        for (int p = k; p < n; p++) {
            if (G[p][k] > max_val) {
                max_val = G[p][k];
                k_p = p;
            }
        }

        // swap row k and row k_p
        double temp;
        for (int i = 0; i < n; i++) {
            temp = G[k][i];
            G[k][i] = G[k_p][i];
            G[k_p][i] = temp;
        }

        


    }

}