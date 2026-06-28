// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int rank, ntasks;
	int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (provided < MPI_THREAD_MULTIPLE){
		printf("Multi-thread not supported by MPI\n");
	}

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	MPI_Comm comms[ntasks];
	for(int i = 0; i < ntasks; i++){
		MPI_Comm_dup(MPI_COMM_WORLD, &comms[i]);
	}

	#pragma omp parallel
	{
    int msg = -1;
	int tid = omp_get_thread_num();
    int tag = 0;
	if (rank == 0) {
        msg = tid;
        for (int i = 1; i < ntasks; i++) {
            MPI_Send(&msg, 1, MPI_INT, i, tag, comms[tid]);
        }
    } else {
        MPI_Recv(&msg, 1, MPI_INT, 0, tag, comms[tid], MPI_STATUS_IGNORE);
        printf("Rank %d thread %d received %d\n", rank, tid, msg);
    }
	}
    MPI_Finalize();
    return 0;
}
