/*
    Parallel Implementation of Lab 4
    now with alternating r
    how is this slower??
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
    double *r_odd, *r_even;
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
    r_even = malloc(nodecount * sizeof(double));
    r_odd = malloc(nodecount * sizeof(double));
    
    iterationcount = 0;
    for ( i = 0; i < nodecount; ++i)
        r_odd[i] = 1.0 / nodecount;
    
    /* INITIALIZE MORE VARIABLES IF NECESSARY */

    int mpi_size;
    int my_rank;
    int my_start;
    int my_end;
    double loc_sum;
    int loc_node;
    int phase_even = 1;
    double error;

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

    do {
        ++iterationcount;

        /* IMPLEMENT ITERATIVE UPDATE */
        
        // update section of r owned by this process
        for (i = 0; i < nodecounts[my_rank]; i++) {
            loc_sum = 0.0;

            if (phase_even) {
                for (j = 0; j < nodehead[my_start+i].num_in_links; j++) {
                    loc_node = nodehead[my_start+i].inlinks[j];
                    loc_sum += r_odd[loc_node] / nodehead[loc_node].num_out_links; // use r_ptr
                }
            } else {
                for (j = 0; j < nodehead[my_start+i].num_in_links; j++) {
                    loc_node = nodehead[my_start+i].inlinks[j];
                    loc_sum += r_even[loc_node] / nodehead[loc_node].num_out_links; // use r_ptr
                }
            }
            
            loc_r[i] = (1.0 - DAMPING_FACTOR) * 1.0/nodecount + DAMPING_FACTOR * loc_sum;   
        }

        // gather result

        if (phase_even) {
            MPI_Allgatherv(
                loc_r, nodecounts[my_rank], MPI_DOUBLE,
                r_even, nodecounts, displacements, MPI_DOUBLE,
                MPI_COMM_WORLD
            );
            error = rel_error(r_even, r_odd, nodecount);
        } else {
            MPI_Allgatherv(
                loc_r, nodecounts[my_rank], MPI_DOUBLE,
                r_odd, nodecounts, displacements, MPI_DOUBLE,
                MPI_COMM_WORLD
            );
            error = rel_error(r_odd, r_even, nodecount);
        }

        phase_even = !phase_even;

    } while(error >= EPSILON);

    // free(loc_r);

    MPI_Finalize();
    GET_TIME(end);

    if (phase_even) {
        Lab4_saveoutput(r_odd, nodecount, end - start);
    } else {
        Lab4_saveoutput(r_even, nodecount, end - start);
    }

    // post processing
    node_destroy(nodehead, nodecount);
    free(r_even); free(r_odd);
    return 0;
}
