#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define N 4

__global__ void matMul(float *A, float *B, float *C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < n && col < n) {
        float s = 0;
        for (int k = 0; k < n; k++) s += A[row * n + k] * B[k * n + col];
        C[row * n + col] = s;
    }
}

int main() {
    size_t bytes = N * N * sizeof(float);
    float *hA = (float*)malloc(bytes), *hB = (float*)malloc(bytes), *hC = (float*)malloc(bytes);
    for (int i = 0; i < N * N; i++) { hA[i] = i % 5; hB[i] = (i % 3) + 1; }

    float *dA, *dB, *dC;
    cudaMalloc(&dA, bytes); cudaMalloc(&dB, bytes); cudaMalloc(&dC, bytes);
    cudaMemcpy(dA, hA, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, hB, bytes, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((N + 15) / 16, (N + 15) / 16);
    matMul<<<blocks, threads>>>(dA, dB, dC, N);
    cudaMemcpy(hC, dC, bytes, cudaMemcpyDeviceToHost);

    printf("Result C:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) printf("%6.1f ", hC[i * N + j]);
        printf("\n");
    }
    cudaFree(dA); cudaFree(dB); cudaFree(dC);
    free(hA); free(hB); free(hC);
    return 0;
}
