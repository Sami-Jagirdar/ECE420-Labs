/*
    Parallel Implementation of Lab 4
    trying with padding instead of allgatherv
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
    double *r_odd, *r_even, *r, *r_pre;
    int i, j;
    int iterationcount;
    double start, end;
    FILE *ip;

    int mpi_size;
    int my_rank;
    int my_start;
    int my_end;

    double loc_sum;
    int loc_node;
    int phase_even = 1;
    double eqn_factor;

    int r_size;
    int loc_nodecount_most;
    int * loc_nodecounts;
    double * loc_r;


    /* INSTANTIATE MORE VARIABLES IF NECESSARY */
    
    // load data 
    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 253;
    }
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);
    if (node_init(&nodehead, 0, nodecount)) return 254;
    
    eqn_factor = (1.0 - DAMPING_FACTOR) * 1.0 / nodecount; // save a computation in the main loop

    // core calculation
    GET_TIME(start);
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    loc_nodecount_most = ceil((double) nodecount / mpi_size);
    r_size = loc_nodecount_most * mpi_size;

    r_even = malloc(r_size * sizeof(double));
    r_odd = malloc(r_size * sizeof(double));

    // initialize arrays    
    iterationcount = 0;
    for ( i = 0; i < nodecount; ++i)
        r_odd[i] = 1.0 / nodecount;

    // figure out nodecounts and displacements for each process
    loc_nodecounts = malloc(mpi_size * sizeof(int));

    for (int i = 0; i < mpi_size-1; i++) {
        loc_nodecounts[i] = loc_nodecount_most;
    }
    loc_nodecounts[mpi_size-1] = nodecount - loc_nodecount_most * (mpi_size - 1);

    // variables for this proc
    loc_r = malloc(loc_nodecount_most * sizeof(double));
    my_start = my_rank * loc_nodecount_most;
    my_end = my_start + loc_nodecounts[my_rank];

    // printf("hello from rank %d of %d, loc n = %d, [%d, %d)\n",
    //     my_rank, mpi_size, loc_nodecounts[my_rank], my_start, my_end);

    do {
        ++iterationcount;

        // for each phase swap which array is r and which is r_pre, so we don't need to reinitialize anything
        if (phase_even) {
            r = r_even; r_pre = r_odd;
        } else {
            r = r_odd; r_pre = r_even;
        }

        /* IMPLEMENT ITERATIVE UPDATE */
        
        // update section of r owned by this process
        for (i = 0; i < loc_nodecounts[my_rank]; i++) {
            loc_sum = 0.0;
            // calculate sum
            for (j = 0; j < nodehead[my_start+i].num_in_links; j++) {
                loc_node = nodehead[my_start+i].inlinks[j];
                loc_sum += r_pre[loc_node] / nodehead[loc_node].num_out_links;
            }
            
            loc_r[i] = eqn_factor + DAMPING_FACTOR * loc_sum;   
        }

        // gather result
        MPI_Allgather(
            loc_r, loc_nodecount_most, MPI_DOUBLE,
            r, loc_nodecount_most, MPI_DOUBLE,
            MPI_COMM_WORLD
        );

        phase_even = !phase_even;

    } while (rel_error(r, r_pre, nodecount) >= EPSILON);

    MPI_Finalize();
    GET_TIME(end);

    Lab4_saveoutput(r, nodecount, end - start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(r_even); free(r_odd); free(loc_r); free(loc_nodecounts);
    return 0;
}
