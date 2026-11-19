#define CL_TARGET_OPENCL_VERSION 120
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define N 1024
#define LOCAL 64

const char *src =
"__kernel void reduce(__global const int *in, __global int *out, __local int *scratch){\n"
"    int gid = get_global_id(0);\n"
"    int lid = get_local_id(0);\n"
"    int lsz = get_local_size(0);\n"
"    scratch[lid] = in[gid];\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    for (int s = lsz / 2; s > 0; s >>= 1) {\n"
"        if (lid < s) scratch[lid] += scratch[lid + s];\n"
"        barrier(CLK_LOCAL_MEM_FENCE);\n"
"    }\n"
"    if (lid == 0) out[get_group_id(0)] = scratch[0];\n"
"}\n";

int main() {
    cl_int err;
    int groups = N / LOCAL;
    int *in = malloc(N * sizeof(int)), *out = malloc(groups * sizeof(int));
    for (int i = 0; i < N; i++) in[i] = 1;   /* expected total = N */

    cl_platform_id plat; cl_device_id dev;
    clGetPlatformIDs(1, &plat, NULL);
    clGetDeviceIDs(plat, CL_DEVICE_TYPE_DEFAULT, 1, &dev, NULL);
    cl_context ctx = clCreateContext(NULL, 1, &dev, NULL, NULL, &err);
    cl_command_queue q = clCreateCommandQueue(ctx, dev, 0, &err);

    cl_mem dIn = clCreateBuffer(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, N * sizeof(int), in, &err);
    cl_mem dOut = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, groups * sizeof(int), NULL, &err);

    cl_program prog = clCreateProgramWithSource(ctx, 1, &src, NULL, &err);
    err = clBuildProgram(prog, 1, &dev, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char log[4096];
        clGetProgramBuildInfo(prog, dev, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
        printf("Build error:\n%s\n", log);
        return 1;
    }
    cl_kernel k = clCreateKernel(prog, "reduce", &err);
    clSetKernelArg(k, 0, sizeof(cl_mem), &dIn);
    clSetKernelArg(k, 1, sizeof(cl_mem), &dOut);
    clSetKernelArg(k, 2, LOCAL * sizeof(int), NULL);   /* __local buffer */

    size_t global = N, local = LOCAL;
    clEnqueueNDRangeKernel(q, k, 1, NULL, &global, &local, 0, NULL, NULL);
    clEnqueueReadBuffer(q, dOut, CL_TRUE, 0, groups * sizeof(int), out, 0, NULL, NULL);

    int total = 0;
    for (int g = 0; g < groups; g++) { printf("group %d partial = %d\n", g, out[g]); total += out[g]; }
    printf("Total = %d (expected %d)\n", total, N);

    clReleaseMemObject(dIn); clReleaseMemObject(dOut);
    clReleaseKernel(k); clReleaseProgram(prog);
    clReleaseCommandQueue(q); clReleaseContext(ctx);
    free(in); free(out);
    return 0;
}
