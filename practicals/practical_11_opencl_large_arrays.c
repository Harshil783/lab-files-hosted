#define CL_TARGET_OPENCL_VERSION 120
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define N (1 << 22)   /* ~4 million elements */

const char *src =
"__kernel void ops(__global const float *a, __global const float *b,\n"
"                  __global float *add, __global float *sub, __global float *mul){\n"
"    int i = get_global_id(0);\n"
"    add[i] = a[i] + b[i];\n"
"    sub[i] = a[i] - b[i];\n"
"    mul[i] = a[i] * b[i];\n"
"}\n";

int main() {
    cl_int err;
    size_t bytes = N * sizeof(float);
    float *a = malloc(bytes), *b = malloc(bytes);
    float *add = malloc(bytes), *sub = malloc(bytes), *mul = malloc(bytes);
    for (int i = 0; i < N; i++) { a[i] = i % 100; b[i] = (i % 50) + 1; }

    cl_platform_id plat; cl_device_id dev;
    clGetPlatformIDs(1, &plat, NULL);
    clGetDeviceIDs(plat, CL_DEVICE_TYPE_DEFAULT, 1, &dev, NULL);
    cl_context ctx = clCreateContext(NULL, 1, &dev, NULL, NULL, &err);
    cl_command_queue q = clCreateCommandQueue(ctx, dev, 0, &err);

    cl_mem dA = clCreateBuffer(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, a, &err);
    cl_mem dB = clCreateBuffer(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, b, &err);
    cl_mem dAdd = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, bytes, NULL, &err);
    cl_mem dSub = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, bytes, NULL, &err);
    cl_mem dMul = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, bytes, NULL, &err);

    cl_program prog = clCreateProgramWithSource(ctx, 1, &src, NULL, &err);
    err = clBuildProgram(prog, 1, &dev, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char log[4096];
        clGetProgramBuildInfo(prog, dev, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
        printf("Build error:\n%s\n", log);
        return 1;
    }
    cl_kernel k = clCreateKernel(prog, "ops", &err);
    clSetKernelArg(k, 0, sizeof(cl_mem), &dA);
    clSetKernelArg(k, 1, sizeof(cl_mem), &dB);
    clSetKernelArg(k, 2, sizeof(cl_mem), &dAdd);
    clSetKernelArg(k, 3, sizeof(cl_mem), &dSub);
    clSetKernelArg(k, 4, sizeof(cl_mem), &dMul);

    size_t global = N;
    clEnqueueNDRangeKernel(q, k, 1, NULL, &global, NULL, 0, NULL, NULL);
    clEnqueueReadBuffer(q, dAdd, CL_TRUE, 0, bytes, add, 0, NULL, NULL);
    clEnqueueReadBuffer(q, dSub, CL_TRUE, 0, bytes, sub, 0, NULL, NULL);
    clEnqueueReadBuffer(q, dMul, CL_TRUE, 0, bytes, mul, 0, NULL, NULL);

    int errors = 0;
    for (int i = 0; i < N; i++)
        if (add[i] != a[i] + b[i] || sub[i] != a[i] - b[i] || mul[i] != a[i] * b[i]) errors++;
    printf("Elements: %d, mismatches: %d\n", N, errors);
    for (int i = 0; i < 5; i++)
        printf("a=%.0f b=%.0f add=%.0f sub=%.0f mul=%.0f\n", a[i], b[i], add[i], sub[i], mul[i]);

    clReleaseMemObject(dA); clReleaseMemObject(dB);
    clReleaseMemObject(dAdd); clReleaseMemObject(dSub); clReleaseMemObject(dMul);
    clReleaseKernel(k); clReleaseProgram(prog);
    clReleaseCommandQueue(q); clReleaseContext(ctx);
    free(a); free(b); free(add); free(sub); free(mul);
    return 0;
}
