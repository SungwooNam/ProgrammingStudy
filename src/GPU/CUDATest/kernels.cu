#include <cuda_runtime.h>
#include "kernels.h"

__global__ void helloworld(const char *in, char *out, int count)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < count)
    {
		out[i] = in[i] + 1;
    }
}


extern "C" void CallHelloWorld( const char *in, char *out, int count)
{
    helloworld<<<1, count>>>(in, out, count);
}