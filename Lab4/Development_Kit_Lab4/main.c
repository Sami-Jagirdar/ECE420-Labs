/*
    Serial Implementation of Lab 4
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

    // core calculation
    MPI_Init(&argc, &argv);

    int mpi_size;
    int my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int my_start;
    int my_end;

    // find nodecounts of each proc
    int* nodecounts = malloc(mpi_size * sizeof(int));
    int* displacements = malloc((mpi_size+1) * sizeof(int));
    int loc_nodecount_ceil = ceil(nodecount / mpi_size);

    if (my_rank != mpi_size - 1) {
        // first processes get equal portions of nodes
        nodecounts[my_rank] = loc_nodecount_ceil;
    } else {
        // final process gets leftover nodes
        nodecounts[my_rank] = nodecount - loc_nodecount_ceil * (mpi_size - 1);
    }
    displacements[my_rank] = loc_nodecount_ceil * my_rank;
    displacements[mpi_size] = nodecounts;
    my_start = displacements[my_rank];
    my_end = my_start + nodecounts[my_rank];

    double *loc_r = malloc(nodecounts[my_rank] * sizeof(double));

    printf("hello from rank %d of %d, loc n = %d, disp=%d, [%d, %d)\n",
        my_rank, mpi_size, nodecounts[my_rank], displacements[my_rank], my_start, my_end);

    if (my_rank == 0) {
        for (i = 0; i < nodecount; i++) {
            r_pre[i] = r[i];
            r[i] = 0;
        }
    }

    do{
        ++iterationcount;
        /* IMPLEMENT ITERATIVE UPDATE */

        MPI_Bcast(r_pre, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(r, nodecount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // printf("rank %d, r[0]=%f\n", my_rank, r[0]);

        for (i = 0; i < loc_nodecount; i++) {
            // update this r
            double sum = 0.0;
            for (j = 0; j < nodehead[my_start+i].num_in_links; j++) {
                int in_node = nodehead[my_start+i].inlinks[j];
                sum += r_pre[in_node] / nodehead[in_node].num_out_links;
            }
            loc_r[i] = (1.0 - DAMPING_FACTOR) * 1.0/nodecount + DAMPING_FACTOR * sum;   
        }

        // gather results at p0
        MPI_Gatherv(
            loc_r, nodecounts[my_rank], MPI_DOUBLE,
            r, nodecounts, displacements, MPI_DOUBLE,
            0, MPI_COMM_WORLD
        );
        if (my_rank == 0) {
            error = rel_error(r, r_pre, nodecount);
        }
        MPI_Bcast(&error, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } while(error >= EPSILON);

    // free(loc_r);

    MPI_Finalize();

    Lab4_saveoutput(r, nodecount, end - start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(r); free(r_pre);
    return 0;
}
