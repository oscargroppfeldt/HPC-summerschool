// SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

#include <cstdio>
#include <omp.h>

int main()
{
    printf("Hello world!\n");
	#ifdef _OPENMP
	int num_t = omp_get_num_threads();
	printf("Number of threads: %d\n",num_t);
	
	int tid = omp_get_thread_num();
	#else
	int tid = -1;
	#endif
    #pragma omp parallel
    {
        printf("Hello from thread %d!\n", tid);
    }

    return 0;
}
