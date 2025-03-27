/*
    Parallel Implementation of Lab 4
    now with alternating r, implemented with pointers
    CURRENT BEST?
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
    int my_displacement;
    int my_nodecount;
    int my_endnode;
    int loc_nodecount_most;

    double * loc_r;
    register double loc_sum; // written to frequently, use register
    int loc_node;
    int phase_even = 1;
    double eqn_factor;

    int * nodecounts;
    int * displacements;

    /* INSTANTIATE MORE VARIABLES IF NECESSARY */
    
    // load data 
    if ((ip = fopen("data_input_meta","r")) == NULL) {
        printf("Error opening the data_input_meta file.\n");
        return 253;
    }
    fscanf(ip, "%d\n", &nodecount);
    fclose(ip);
    if (node_init(&nodehead, 0, nodecount)) return 254;
    
    // initialize arrays
    r_even = malloc(nodecount * sizeof(double));
    r_odd = malloc(nodecount * sizeof(double));
    
    iterationcount = 0;
    for ( i = 0; i < nodecount; ++i)
        r_odd[i] = 1.0 / nodecount;

    eqn_factor = (1.0 - DAMPING_FACTOR) * 1.0 / nodecount; // save a computation in the main loop
    
    GET_TIME(start);
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // figure out nodecounts and displacements for each process
    nodecounts = malloc(mpi_size * sizeof(int));
    displacements = malloc(mpi_size * sizeof(int));

    loc_nodecount_most = nodecount / mpi_size; // largest loc_nodecount, to be used by all except p=mpi_size-1

    for (int i = 0; i < mpi_size-1; i++) {
        nodecounts[i] = loc_nodecount_most;
        displacements[i] = loc_nodecount_most * i;
    }
    loc_node = nodehead[i].inlinks[j];

    // final process takes leftover nodes
    nodecounts[mpi_size-1] = nodecount - loc_nodecount_most * (mpi_size - 1);
    displacements[mpi_size-1] = loc_nodecount_most * (mpi_size - 1);

    // variables for this proc
    loc_r = malloc(nodecounts[my_rank] * sizeof(double));
    my_displacement = displacements[my_rank];
    my_nodecount = nodecounts[my_rank];
    my_endnode = my_nodecount + my_displacement;

    // core calculation
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
        for (i = my_displacement; i < my_endnode; i++) {
            loc_sum = 0.0;
            // calculate sum
            for (j = 0; j < nodehead[i].num_in_links; j++) {
                loc_node = nodehead[i].inlinks[j];
                loc_sum += r_pre[loc_node] / nodehead[loc_node].num_out_links;
            }
            
            loc_r[i - my_displacement] = eqn_factor + DAMPING_FACTOR * loc_sum;   
        }

        // gather result
        MPI_Allgatherv(
            loc_r, my_nodecount, MPI_DOUBLE,
            r, nodecounts, displacements, MPI_DOUBLE,
            MPI_COMM_WORLD
        );

        phase_even = !phase_even;

    } while (rel_error(r, r_pre, nodecount) >= EPSILON);

    MPI_Finalize();
    GET_TIME(end);

    Lab4_saveoutput(r, nodecount, end - start);

    // post processing
    node_destroy(nodehead, nodecount);
    free(r_even); free(r_odd); free(loc_r);
    free(nodecounts); free(displacements);
    return 0;
}
