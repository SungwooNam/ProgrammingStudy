#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <CL\cl.h>
#include <vector>
#include <string>
#include <fstream>

#define SUCCESS 0
#define FAILURE 1

using namespace std;

BOOST_AUTO_TEST_SUITE( TestHelloWorld )
	

BOOST_AUTO_TEST_CASE( TestUsingGPU ) 
{  
	cl_uint numPlatforms;
	cl_platform_id platform = NULL;
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS) 
		return;

	if(numPlatforms > 0)
	{
		vector<cl_platform_id> platforms( numPlatforms );
		clGetPlatformIDs(numPlatforms, &platforms[0], NULL);

		for( size_t i=0; i<platforms.size(); ++i) 
		{
			char platformName[255], platformVersion[255];
			size_t infoSize;
			clGetPlatformInfo( platforms[i], CL_PLATFORM_NAME, 255, platformName, &infoSize );
			clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION, 255, platformVersion, &infoSize );
			printf( "found platform %s : %s\n", platformName, platformVersion );
		}

		platform = platforms[0];
	}

	cl_uint numDevices = 0;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);	
	if (numDevices == 0)	
		return;

	vector<cl_device_id> devices( numDevices );
	clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, numDevices, &devices[0], NULL);
	for( size_t i=0; i<devices.size(); ++i )
	{
		char deviceName[255];
		size_t infoSize;
		clGetDeviceInfo( devices[i], CL_DEVICE_NAME, 255, deviceName, &infoSize );
		printf( "found device : %s\n", deviceName );
	}
	

	cl_context context = clCreateContext(NULL,1, &devices[0],NULL,NULL,NULL);
	
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

	ifstream programFile( "Kernel_HelloWorld.cl" );
	string source( std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));
	const char *sourcePtr = source.c_str();
	size_t sourceLength = source.length() +1; 
	cl_program program = clCreateProgramWithSource(context, 1, &sourcePtr, &sourceLength, NULL);
	
	status=clBuildProgram(program, 1,&devices[0],NULL,NULL,NULL);

	string input( "GdkknVnqkc" );
	size_t inputLength = input.length() + 1;
	vector<char> output( inputLength, 0 );
	printf( "input string : %s\n", input.c_str() );


	cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, inputLength * sizeof(char),(void *) input.c_str(), NULL);
	cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY , inputLength * sizeof(char), NULL, NULL);

	cl_kernel kernel = clCreateKernel(program,"helloworld", NULL);

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);
	
	size_t global_work_size[1] = { input.length() };
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, inputLength, &output[0], 0, NULL, NULL);

	printf( "output string : " );
	for( size_t i=0; i<output.size()-1; ++i)
	{
		printf( "%c", output[i] );
	}
	printf( "\n" );

	status = clReleaseKernel(kernel);				//Release kernel.
	status = clReleaseProgram(program);				//Release the program object.
	status = clReleaseMemObject(inputBuffer);		//Release mem object.
	status = clReleaseMemObject(outputBuffer);
	status = clReleaseCommandQueue(commandQueue);	//Release  Command queue.
	status = clReleaseContext(context);				//Release context.
};

BOOST_AUTO_TEST_CASE( TestUsingCPU ) 
{  
	cl_uint numPlatforms;
	cl_platform_id platform = NULL;
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS) 
		return;

	if(numPlatforms > 0)
	{
		vector<cl_platform_id> platforms( numPlatforms );
		clGetPlatformIDs(numPlatforms, &platforms[0], NULL);

		for( size_t i=0; i<platforms.size(); ++i) 
		{
			char platformName[255], platformVersion[255];
			size_t infoSize;
			clGetPlatformInfo( platforms[i], CL_PLATFORM_NAME, 255, platformName, &infoSize );
			clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION, 255, platformVersion, &infoSize );
			printf( "found platform %s : %s\n", platformName, platformVersion );
		}

		platform = platforms[0];
	}

	cl_uint numDevices = 0;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);	
	if (numDevices == 0)	
		return;

	vector<cl_device_id> devices( numDevices );
	clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, numDevices, &devices[0], NULL);
	for( size_t i=0; i<devices.size(); ++i )
	{
		char deviceName[255];
		size_t infoSize;
		clGetDeviceInfo( devices[i], CL_DEVICE_NAME, 255, deviceName, &infoSize );
		printf( "found device : %s\n", deviceName );
	}
	

	cl_context context = clCreateContext(NULL,1, &devices[0],NULL,NULL,NULL);
	
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

	ifstream programFile( "Kernel_HelloWorld.cl" );
	string source( std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));
	const char *sourcePtr = source.c_str();
	size_t sourceLength = source.length() +1; 
	cl_program program = clCreateProgramWithSource(context, 1, &sourcePtr, &sourceLength, NULL);
	
	status=clBuildProgram(program, 1,&devices[0],NULL,NULL,NULL);

	string input( "GdkknVnqkc" );
	size_t inputLength = input.length() + 1;
	vector<char> output( inputLength, 0 );
	printf( "input string : %s\n", input.c_str() );


	cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, inputLength * sizeof(char),(void *) input.c_str(), NULL);
	cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY , inputLength * sizeof(char), NULL, NULL);

	cl_kernel kernel = clCreateKernel(program,"helloworld", NULL);

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);
	
	size_t global_work_size[1] = { input.length() };
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);

	status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, inputLength, &output[0], 0, NULL, NULL);

	printf( "output string : " );
	for( size_t i=0; i<output.size()-1; ++i)
	{
		printf( "%c", output[i] );
	}
	printf( "\n" );

	status = clReleaseKernel(kernel);				//Release kernel.
	status = clReleaseProgram(program);				//Release the program object.
	status = clReleaseMemObject(inputBuffer);		//Release mem object.
	status = clReleaseMemObject(outputBuffer);
	status = clReleaseCommandQueue(commandQueue);	//Release  Command queue.
	status = clReleaseContext(context);				//Release context.
};



BOOST_AUTO_TEST_SUITE_END()