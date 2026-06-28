// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <vector>
#include <mpi.h>

#define MAX_PRINT_SIZE 12

void init_buffer(std::vector<int> &buffer);
void print_buffer(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int size, rank, buf_size=12;
    std::vector<int> buf(buf_size);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Initialize message buffer */
    init_buffer(buf);

    /* Print data that will be sent */
    print_buffer(buf);

    /* Start timing */
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
	
	int tag = 0;
    /* Send everywhere */
    // TODO: Implement the broadcast of the array buf
	/*
	 * 2 i
	if(0 == rank){
		for(int dst{1}; dst < size; dst++){
			MPI_Send(buf.data(), buf_size, MPI_INT, dst, tag, MPI_COMM_WORLD);
		}
	} else {
		MPI_Status stat;
		MPI_Recv(buf.data(), buf_size, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
	}
	*/
    /* End timing */
	// 2 ii
	//MPI_Bcast(buf.data(), buf_size, MPI_INT, rank, MPI_COMM_WORLD);
    
	/* 3 i
	int block_size = buf_size / size;
	int num_blocks = buf_size / block_size; // size
	if (0 == rank){
		for (int i{1}; i < num_blocks; i++){
			MPI_Send(buf.data()+i*block_size, block_size, MPI_INT, i,tag, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Status stat;
		MPI_Recv(buf.data(), block_size, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
	}
	*/
	// 3 ii
	auto recv_buf = buf;
	MPI_Scatter(buf.data(),	buf_size / size, MPI_INT, recv_buf.data(), buf_size / size, MPI_INT, 0, MPI_COMM_WORLD);

	double t1 = MPI_Wtime();

    /* Print data that was received */
    print_buffer(recv_buf);
    if (rank == 0) {
        printf("Time elapsed: %6.8f s\n", t1 - t0);
    }

    MPI_Finalize();
    return 0;
}


void init_buffer(std::vector<int> &buffer)
{
    int rank;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = i;
        }
    } else {
        for (int i = 0; i < buffersize; i++) {
            buffer[i] = -1;
        }
    }
}


void print_buffer(std::vector<int> &buffer)
{
    int rank, size;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> printbuffer(buffersize * size);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < size; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < MAX_PRINT_SIZE; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            if (MAX_PRINT_SIZE < buffersize) {
                printf(" ...");
            }
            printf("\n");
        }
        printf("\n");
    }
}
