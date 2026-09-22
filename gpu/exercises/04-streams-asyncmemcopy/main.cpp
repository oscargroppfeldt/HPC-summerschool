// SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
//
// SPDX-License-Identifier: MIT

/*
 * This code in its current form uses the default stream
 * Task:
 * - Place kernel_{a,b,c} in separate streams and execute them asynchronously
 * - Validate that kernels are executing concurrently with `run_tue ... rocprof --hip-trace ./<your->
 *   - Open chromium url chrome://tracing or https://ui.perfetto.dev, open file "results.json"
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "error_checking.hpp"

// GPU kernel definition
__global__ void kernel_a(float *a, int n)
{
  int tid = threadIdx.x + blockIdx.x * blockDim.x;

  // Evaluate the trigonometric identity
  // sin^2(x) + cos^2(x) = 1
  // Very light kernel, one sin/cos evaluation per element
  if (tid < n) {
    float x = 0.001f * float(tid % 1000);
    float s = sinf(x);
    float c = cosf(x);

    a[tid] = s * s + c * c;
  }
}


__global__ void kernel_b(float *b, int n)
{
  int tid = threadIdx.x + blockIdx.x * blockDim.x;

  // Heavy kernel: repeatedly updates x using sine, cosine, and arctangent
  // Converges to 1.313534
  if (tid < n) {
    float x = 0.001f * float(tid % 1000) + 1.0f;

    for (int i = 0; i < 200; ++i) {
      x = sinf(x) + cosf(x) + 0.1f * atanf(x);
    }

    b[tid] = x;
  }
}

__global__ void kernel_c(float *c, int n)
{
  int tid = threadIdx.x + blockIdx.x * blockDim.x;

  if (tid < n) {
    float x = 0.001f * float(tid % 1000);

    // Fixed-point iteration for cos(x) = x.
    // Converges to ~0.739085
    // Medium
    for (int i = 0; i < 50; ++i) {
      x = cosf(x);
    }

    c[tid] = x;
  }
}

int main() {
  constexpr size_t N = 1<<24; // ~64 MiB array

  constexpr int blocksize = 256;
  constexpr int gridsize =(N-1+blocksize)/blocksize;
  constexpr size_t N_bytes = N*sizeof(float);

  // Host & device pointers
  float *a; float *d_a;
  float *b; float *d_b;
  float *c; float *d_c;

  // Host allocations
  HIP_ERRCHK(hipHostMalloc((void**)&a, N_bytes));
  HIP_ERRCHK(hipHostMalloc((void**)&b, N_bytes));
  HIP_ERRCHK(hipHostMalloc((void**)&c, N_bytes));

	hipStream_t s_a, s_b, s_c;
	HIP_ERRCHK(hipStreamCreate(&s_a));
	HIP_ERRCHK(hipStreamCreate(&s_b));
	HIP_ERRCHK(hipStreamCreate(&s_c));
  // Device allocations
  HIP_ERRCHK(hipMalloc((void**)&d_a, N_bytes));
  HIP_ERRCHK(hipMalloc((void**)&d_b, N_bytes));
  HIP_ERRCHK(hipMalloc((void**)&d_c, N_bytes));

  // warmup
  kernel_c<<<gridsize, blocksize>>>(d_a, N);
  HIP_ERRCHK(hipMemcpy(a, d_a, N_bytes/100, hipMemcpyDefault));
  HIP_ERRCHK(hipDeviceSynchronize());

  // Execute kernels in sequence
  kernel_a<<<gridsize, blocksize,0,s_a>>>(d_a, N);
  HIP_ERRCHK(hipGetLastError());

  kernel_b<<<gridsize, blocksize,0,s_b>>>(d_b, N);
  HIP_ERRCHK(hipGetLastError());

  kernel_c<<<gridsize, blocksize,0,s_c>>>(d_c, N);
  HIP_ERRCHK(hipGetLastError());

  // Copy results back (in the default stream with hipMemCpy)
  HIP_ERRCHK(hipMemcpyAsync(a, d_a, N_bytes, hipMemcpyDefault,s_a));  
  HIP_ERRCHK(hipStreamSynchronize(s_a));
  HIP_ERRCHK(hipMemcpyAsync(b, d_b, N_bytes, hipMemcpyDefault,s_b));
  HIP_ERRCHK(hipStreamSynchronize(s_b));
  HIP_ERRCHK(hipMemcpyAsync(c, d_c, N_bytes, hipMemcpyDefault,s_c));
  HIP_ERRCHK(hipStreamSynchronize(s_c));

  for (int i = 0; i < 10; ++i) printf("%f ", a[i]);
  printf("\n");

  for (int i = 0; i < 10; ++i) printf("%f ", b[i]);
  printf("\n");

  for (int i = 0; i < 10; ++i) printf("%f ", c[i]);
  printf("\n");
  // Free device and host memory allocations
  HIP_ERRCHK(hipFreeAsync(d_a, s_a));
  HIP_ERRCHK(hipFreeAsync(d_b, s_b));
  HIP_ERRCHK(hipFreeAsync(d_c, s_c));


  HIP_ERRCHK(hipStreamDestroy(s_a));
  HIP_ERRCHK(hipStreamDestroy(s_b));
  HIP_ERRCHK(hipStreamDestroy(s_c));
  HIP_ERRCHK(hipHostFree(a));
  HIP_ERRCHK(hipHostFree(b));
  HIP_ERRCHK(hipHostFree(c));
}
