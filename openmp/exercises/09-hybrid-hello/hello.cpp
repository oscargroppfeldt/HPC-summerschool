// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	int provided, required=MPI_THREAD_FUNNELED;
	MPI_Init_thread(&argc, &argv, required, &provided);
	int mpi_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        printf("Hello from thread %d in process %d!\n", tid, mpi_rank);
    }

	if (mpi_rank == 0) {
        printf("\nProvided thread support level: %d\n", provided);
        printf("  %d - MPI_THREAD_SINGLE\n", MPI_THREAD_SINGLE);
        printf("  %d - MPI_THREAD_FUNNELED\n", MPI_THREAD_FUNNELED);
        printf("  %d - MPI_THREAD_SERIALIZED\n", MPI_THREAD_SERIALIZED);
    	printf("  %d - MPI_THREAD_MULTIPLE\n", MPI_THREAD_MULTIPLE);
    }
	MPI_Finalize();
    return 0;
}
