#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define N 16

// each thread reads in[i] from global memory and writes out[i]
__global__ void rw(int *in, int *out, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) out[i] = in[i] * 2;
}

int main() {
    size_t bytes = N * sizeof(int);
    int *h_in = (int*)malloc(bytes), *h_out = (int*)malloc(bytes);
    for (int i = 0; i < N; i++) h_in[i] = i + 1;

    int *d_in, *d_out;
    cudaMalloc(&d_in, bytes); cudaMalloc(&d_out, bytes);
    cudaMemcpy(d_in, h_in, bytes, cudaMemcpyHostToDevice);   // write to device
    cudaMemset(d_out, 0, bytes);

    rw<<<(N + 7) / 8, 8>>>(d_in, d_out, N);

    cudaMemcpy(h_out, d_out, bytes, cudaMemcpyDeviceToHost); // read from device
    for (int i = 0; i < N; i++) printf("in[%d]=%d  out[%d]=%d\n", i, h_in[i], i, h_out[i]);

    cudaFree(d_in); cudaFree(d_out); free(h_in); free(h_out);
    return 0;
}
