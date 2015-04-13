#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <cuda_runtime.h>

#include "kernels.h"

using namespace std;

BOOST_AUTO_TEST_SUITE( TestHelloWorld )
	

BOOST_AUTO_TEST_CASE( TestUsingGPU ) 
{  
    cudaError_t err = cudaSuccess;

	string input( "GdkknVnqkc" );
	size_t inputLength = input.length() + 1;
	vector<char> output( inputLength, 0 );
	printf( "input string : %s\n", input.c_str() );

	char *devInput = NULL;
    err = cudaMalloc((void **)&devInput, inputLength);
    if (err != cudaSuccess) return;

	char *devOutput = NULL;
    err = cudaMalloc((void **)&devOutput, inputLength);
    if (err != cudaSuccess) return;

    err = cudaMemcpy(devInput, &input[0], inputLength, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) return;

	CallHelloWorld( devInput, devOutput, inputLength );
    err = cudaGetLastError();
    if (err != cudaSuccess) return;

    err = cudaMemcpy( &output[0], devOutput, inputLength, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) return;

	printf( "output string : " );
	for( size_t i=0; i<output.size()-1; ++i)
	{
		printf( "%c", output[i] );
	}
	printf( "\n" );


    err = cudaFree(devInput);
    if (err != cudaSuccess) return;

	err = cudaFree(devInput);
    if (err != cudaSuccess) return;
};


BOOST_AUTO_TEST_SUITE_END()