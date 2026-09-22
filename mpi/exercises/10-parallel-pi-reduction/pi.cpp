// SPDX-FileCopyrightText: 2019 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <cmath>
#include <mpi.h>
constexpr int n = 840;

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int istart = rank * n / size + 1;
  int istop = (rank + 1) * n / size;
  printf("Size: %d, rank: %d", size, rank);
  printf("Computing approximation to pi with N=%d, starting at %d, ending at %d, in total %d entires\n", n, istart, istop, istop - istart + 1);

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }
  MPI_Allreduce(MPI_IN_PLACE,&pi, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  if (0 == rank){
	  pi *= 4./n;
	  printf("PI=%f",pi);
	}
  MPI_Finalize();
}
