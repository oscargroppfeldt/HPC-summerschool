// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <iostream>
extern "C" {
    #include <mpi.h>
}
int main(int argc, char *argv[])
{
    // TODO: say hello! in parallel
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char proc_name[MPI_MAX_PROCESSOR_NAME];
    int* proc_name_len;
    int ret_code = MPI_Get_processor_name(proc_name, proc_name_len);

    std::cout << "My rank is " << rank << " from proc " << proc_name << std::endl;
    if (0 == rank){
        std::cout << "Total number of MPI ranks: " << size << std::endl;
    } else if (42 == rank) {
        std::cout << "I'm the Answer to the Ultimate Question of Life, the Universe, and Everything!" << std::endl;
    } else if ((size - 1) == rank){
        std::cout << "I'm the last but not least" << std::endl;
    }
    MPI_Finalize();
}

