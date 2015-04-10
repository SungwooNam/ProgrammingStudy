#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <cuda_runtime.h>

extern "C" void CallHelloWorld( const char *in, char *out, int count);

#endif