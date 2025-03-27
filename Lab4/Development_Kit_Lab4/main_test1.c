/*
    Version where p0 shifts over r arrays and then broadcasts (slower)
*/

#define LAB4_EXTEND

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab4_IO.h"
#include "timer.h"
#include <mpi.h>

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

int main (int argc, char* argv[]){

    // instantiate variables
    struct node *nodehead;
    int nodecount;
    double *r, *r_pre;
    int i, j;
    int iterationcount;
    double start, end;
    FILE *ip;

    int loc_nodecount;

    /* INSTANTIATE MORE VARIABLES IF NECESSARY */
    
    // load data 
    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 253;
    }
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);
    if (node_init(&nodehead, 0, nodecount)) return 254;
    
    // initialize variables
    r = malloc(nodecount * sizeof(double));
    r_pre = malloc(nodecount * sizeof(double));
    
    iterationcount = 0;
    for ( i = 0; i < nodecount; ++i)
        r[i] = 1.0 / nodecount;
    
    /* INITIALIZE MORE VARIABLES IF NECESSARY */

    printf("nodecount=%d\n", nodecount);
    double error;
    int mpi_size;
    int my_rank;
    int my_start;
    int my_end;

    // core calculation
    GET_TIME(start);
    MPI_Init(&argc, &argv);

    int* nodecounts = malloc(mpi_size * sizeof(int));
    int* displacements = malloc(mpi_size * sizeof(int));

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // set nodecounts of each proc
    if (my_rank == 0) {
        int loc_nodecount_ceil = ceil(nodecount / mpi_size);

        for (int i = 0; i < mpi_size-1; i++) {
            nodecounts[i] = loc_nodecount_ceil;
            displacements[i] = loc_nodecount_ceil * i;
        }
        nodecounts[mpi_size-1] = nodecount - loc_nodecount_ceil * (mpi_size - 1);
        displacements[mpi_size-1] = loc_nodecount_ceil * (mpi_size - 1);
    }
    MPI_Bcast(nodecounts, mpi_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(displacements, mpi_size, MPI_INT, 0, MPI_COMM_WORLD);

    // calculate variables for this proc
    my_start = displacements[my_rank];
    my_end = my_start + nodecounts[my_rank];

    double *loc_r = malloc(nodecounts[my_rank] * sizeof(double));

    printf("hello from rank %d of %d, loc n = %d, disp=%d, [%d, %d)\n",
        my_rank, mpi_size, nodecounts[my_rank], displacements[my_rank], my_start, my_end);

    MPI_Barrier(MPI_COMM_WORLD);

    do{
        ++iterationcount;
        /* IMPLEMENT ITERATIVE UPDATE */

        // shift over arrays
        if (my_rank == 0) {
            for (i = 0; i < nodecount; i++) {
                r_pre[i] = r[i];
                r[i] = 1;
            }
        }
        MPI_Bcast(r_pre, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(r, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // printf("0rank %d, r[0]=%f\n", my_rank, r[0]);

        // update section of r owned by this process
        for (i = 0; i < nodecounts[my_rank]; i++) {
            // update this 
            double sum = 0.0;
            for (j = 0; j < nodehead[my_start+i].num_in_links; j++) {
                int in_node = nodehead[my_start+i].inlinks[j];
                sum += r_pre[in_node] / nodehead[in_node].num_out_links;
            }
            loc_r[i] = (1.0 - DAMPING_FACTOR) * 1.0/nodecount + DAMPING_FACTOR * sum;   
        }

        MPI_Allgatherv(
            loc_r, nodecounts[my_rank], MPI_DOUBLE,
            r, nodecounts, displacements, MPI_DOUBLE,
            MPI_COMM_WORLD
        );

    } while(rel_error(r, r_pre, nodecount) >= EPSILON);

    // free(loc_r);

    MPI_Finalize();
    GET_TIME(end);

    Lab4_saveoutput(r, nodecount, end - start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(r); free(r_pre);
    return 0;
}
