#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <CL/cl.hpp>
#include "OpenCLWorkBench.h"
#include <boost/cstdint.hpp>

using namespace std;
using namespace OpenCLWorkBench;

BOOST_AUTO_TEST_SUITE( TestPitchComparison )
	
BOOST_AUTO_TEST_CASE( TestPitchComparison00 ) 
{  
	Workbench task;
	
	try {
		task.ConnectDevice( CL_DEVICE_TYPE_GPU )
			.BuildContext()
			.BuildProgram( "KernelGPU_Inspection.cl" );

		cl::Kernel kernelPitch0( task.GetProgram(), "pitch_comprison_0" );

		int width = 12*1024, height = 512;
		int left = 10, top =0, right = width-10, bottom = height-1;
		int pitch = 10;

		vector<uint8_t> src( width*height, 0 );
		src[ 10 ] = 50;
		src[ 20 ] = 150;
		src[ 30 ] = 50;
		src[ 10 + (height-1)*width ] = 50;
		src[ 20 + (height-1)*width ] = 150;
		src[ 30 + (height-1)*width ] = 50;

		vector<uint8_t> dst( width*height, 0 );

		cl::CommandQueue queue( task.GetContext(), task.GetConnectedDevices()[0] );

		cl::Buffer devSrc( task.GetContext(), CL_MEM_READ_ONLY, width*height );
		cl::Buffer devDst( task.GetContext(), CL_MEM_WRITE_ONLY, width*height );

		cl::UserEvent kickOff( task.GetContext() );
		cl::Event e1, e2, e3;

		vector<cl::Event> vKickOff; vKickOff.push_back( kickOff );
		queue.enqueueWriteBuffer( devSrc, CL_FALSE, 0, width*height, src.data(), &vKickOff, &e1 );

		kernelPitch0.setArg( 0, devSrc );
		kernelPitch0.setArg( 1, devDst );
		kernelPitch0.setArg( 2, width );
		kernelPitch0.setArg( 3, height );
		kernelPitch0.setArg( 4, left );
		kernelPitch0.setArg( 5, top );
		kernelPitch0.setArg( 6, right );
		kernelPitch0.setArg( 7, bottom );
		kernelPitch0.setArg( 8, pitch );

		const int LOCAL_WIDTH = 64, LOCAL_HEIGHT = 4;
		cl::NDRange localws( LOCAL_WIDTH, LOCAL_HEIGHT );
		cl::NDRange globalws( 
			LOCAL_WIDTH * (int)ceil( (double)(right-left+1) / (double)LOCAL_WIDTH ), 
			LOCAL_HEIGHT * (int)ceil( (double)(bottom-top+1) / (double)LOCAL_HEIGHT )
		);

		vector<cl::Event> ve1; ve1.push_back( e1 );
		queue.enqueueNDRangeKernel( kernelPitch0, cl::NullRange, globalws, localws, &ve1, &e2 );

		vector<cl::Event> ve2; ve2.push_back( e2 );
		queue.enqueueReadBuffer( devDst, CL_FALSE, 0, width*height, dst.data(), &ve2, &e3  );

		kickOff.setStatus( CL_COMPLETE );
		e3.wait();

		BOOST_CHECK_EQUAL( dst[ 10 ], 50 );
		BOOST_CHECK_EQUAL( dst[ 20 ], 200 );
		BOOST_CHECK_EQUAL( dst[ 30 ], 50 );
		BOOST_CHECK_EQUAL( dst[ 10 + (height-1)*width ], 50 );
		BOOST_CHECK_EQUAL( dst[ 20 + (height-1)*width ], 200 );
		BOOST_CHECK_EQUAL( dst[ 30 + (height-1)*width ], 50 );
	}
	catch( cl::Error e ) {
		cout << e.what() << ": Error Code " << e.err() << endl;
	}
	catch( WorkbenchException e ) 
	{
		cout << e.what() << endl;
		cout << task.GetBuildLog() << endl;
	}
}

BOOST_AUTO_TEST_CASE( TestPitchComparison01 ) 
{  
	Workbench task;
	
	try {
		task.ConnectDevice( CL_DEVICE_TYPE_CPU )
			.BuildContext()
			.BuildProgram( "KernelCPU_Inspection.cl" );
		 
		cl::Kernel kernelPitch0( task.GetProgram(), "pitch_comparion_0" );

		int width = 12*1024, height = 512;
		int left = 10, top =0, right = width-10, bottom = height-1;
		float pitch = 9.5;

		cl::CommandQueue queue( task.GetContext(), task.GetConnectedDevices()[0] );

		cl::Buffer devSrc( task.GetContext(), CL_MEM_READ_ONLY, width*height );
		cl::Buffer devDst( task.GetContext(), CL_MEM_WRITE_ONLY, width*height );

		uint8_t* src = (uint8_t*)queue.enqueueMapBuffer( devSrc, CL_TRUE, CL_MAP_WRITE, 0, width*height );
		src[ 10 ] = 50;  src[ 11 ] = 100; 
		src[ 20 ] = 125;
		src[ 29 ] = 50;	 src[ 30 ] = 100; 
		queue.enqueueUnmapMemObject( devSrc, src );

		int integerPitch = (int)(pitch);
		short toFloor = (short)( ((pitch - integerPitch) * 0x7F)+0.5f);
		short toCeiling = (short)( 0x7F - toFloor );
		int cpuCount = 4;
		int blockWidth = (int)ceil( (double)(right - left + 1 ) / cpuCount );

		kernelPitch0.setArg( 0, devSrc );
		kernelPitch0.setArg( 1, devDst );
		kernelPitch0.setArg( 2, width );
		kernelPitch0.setArg( 3, height );
		kernelPitch0.setArg( 4, left );
		kernelPitch0.setArg( 5, top );
		kernelPitch0.setArg( 6, right );
		kernelPitch0.setArg( 7, bottom );
		kernelPitch0.setArg( 8, blockWidth );
		kernelPitch0.setArg( 9, integerPitch );
		kernelPitch0.setArg( 10, toCeiling );
		kernelPitch0.setArg( 11, toFloor );

		cl::NDRange globalws( cpuCount );
		cl::NDRange localws( 1 );

		cl::Event ev = cl::Event();
		queue.enqueueNDRangeKernel( kernelPitch0, cl::NullRange, globalws, localws, NULL, &ev );
		queue.finish();

		uint8_t* dst = (uint8_t*)queue.enqueueMapBuffer( devDst, CL_TRUE, CL_MAP_READ, 0, width*height );
		BOOST_CHECK_EQUAL( dst[ 20 ], 50 );
		queue.enqueueUnmapMemObject( devDst, dst );

	}
	catch( cl::Error e ) {
		cout << e.what() << ": Error Code " << e.err() << endl;
		cout << task.GetBuildLog() << endl;
	}
	catch( WorkbenchException e ) 
	{
		cout << e.what() << endl;
		cout << task.GetBuildLog() << endl;
	} 
}  

BOOST_AUTO_TEST_CASE( TestPitchThreshold00 ) 
{  
	Workbench task;
	
	try {
		task.ConnectDevice( CL_DEVICE_TYPE_GPU )
			.BuildContext()
			.BuildProgram( "KernelGPU_Inspection.cl" );

		cl::Kernel kernelPitch0( task.GetProgram(), "pitch_threshold_0" );

		int width = 12*1024, height = 512;
		int left = 10, top =0, right = width-10, bottom = height-1;
		float pitch = 10.0;


		vector<uint8_t> src( width*height, 0 );
		src[ 10 ] = 50;
		src[ 20 ] = 150;
		src[ 30 ] = 50;
		src[ 10 + (height-1)*width ] = 50;
		src[ 20 + (height-1)*width ] = 150;
		src[ 30 + (height-1)*width ] = 50;

		vector<uint8_t> dst( width*height/8, 0 );

		cl::CommandQueue queue( task.GetContext(), task.GetConnectedDevices()[0] );

		cl::Buffer devSrc( task.GetContext(), CL_MEM_READ_ONLY, width*height );
		cl::Buffer devDst( task.GetContext(), CL_MEM_WRITE_ONLY, width*height/8 );

		cl::UserEvent kickOff( task.GetContext() );
		cl::Event e1, e2, e3;

		vector<cl::Event> vKickOff; vKickOff.push_back( kickOff );
		queue.enqueueWriteBuffer( devSrc, CL_FALSE, 0, width*height, src.data(), &vKickOff, &e1 );

		const int LOCAL_WIDTH = 64, LOCAL_HEIGHT = 4;
		cl::NDRange localws( LOCAL_WIDTH, LOCAL_HEIGHT );
		cl::NDRange globalws( 
			LOCAL_WIDTH * (int)ceil( (double)(right-left+1) / (double)LOCAL_WIDTH ), 
			LOCAL_HEIGHT * (int)ceil( (double)(bottom-top+1) / (double)LOCAL_HEIGHT )
		);

		int integerPitch = (int)(pitch);
		float toCeiling = pitch - (int)pitch;
		float toFloor = 1.0f - toCeiling;
		uint8_t threshold = 10;

		kernelPitch0.setArg( 0, devSrc );
		kernelPitch0.setArg( 1, devDst );
		kernelPitch0.setArg( 2, width );
		kernelPitch0.setArg( 3, height );
		kernelPitch0.setArg( 4, left );
		kernelPitch0.setArg( 5, top );
		kernelPitch0.setArg( 6, right );
		kernelPitch0.setArg( 7, bottom );
		kernelPitch0.setArg( 8, integerPitch );
		kernelPitch0.setArg( 9, toCeiling );
		kernelPitch0.setArg( 10, toFloor );
		kernelPitch0.setArg( 11, LOCAL_WIDTH*LOCAL_HEIGHT, NULL );
		kernelPitch0.setArg( 12, threshold );

		vector<cl::Event> ve1; ve1.push_back( e1 );
		queue.enqueueNDRangeKernel( kernelPitch0, cl::NullRange, globalws, localws, &ve1, &e2 );

		vector<cl::Event> ve2; ve2.push_back( e2 );
		queue.enqueueReadBuffer( devDst, CL_FALSE, 0, width*height/8, dst.data(), &ve2, &e3  );

		kickOff.setStatus( CL_COMPLETE );
		e3.wait();

	}
	catch( cl::Error e ) {
		cout << e.what() << ": Error Code " << e.err() << endl;
	}
	catch( WorkbenchException e ) 
	{
		cout << e.what() << endl;
		cout << task.GetBuildLog() << endl;
	}
}

void FillUpPitchThresholdArg( 
	cl::Kernel& kernel, cl::Buffer& src, cl::Buffer& dst, 
	int width, int height,
	int left, int top, int right, int bottom,
	float pitch, uint8_t threshold, int workspace_size )
{
	int integerPitch = (int)(pitch);
	float toCeiling = pitch - (int)pitch;
	float toFloor = 1.0f - toCeiling;

	kernel.setArg( 0, src );
	kernel.setArg( 1, dst );
	kernel.setArg( 2, width );
	kernel.setArg( 3, height );
	kernel.setArg( 4, left );
	kernel.setArg( 5, top );
	kernel.setArg( 6, right );
	kernel.setArg( 7, bottom );
	kernel.setArg( 8, integerPitch );
	kernel.setArg( 9, toCeiling );
	kernel.setArg( 10, toFloor );
	kernel.setArg( 11, workspace_size, NULL );
	kernel.setArg( 12, threshold );
}

BOOST_AUTO_TEST_CASE( TestPitchThreshold01 ) 
{  
	Workbench task;
	
	try {
		task.ConnectDevice( CL_DEVICE_TYPE_GPU )
			.BuildContext()
			.BuildProgram( "KernelGPU_Inspection.cl" );

		cl::Kernel kernelPitch0( task.GetProgram() , "pitch_threshold_0" );
		cl::Kernel kernelPitch1( task.GetProgram() , "pitch_threshold_0" );
		 
		int width = 12*1024, height = 512;
		int left = 10, top =0, right = width-10, bottom = height-1;
		float pitch = 10.0;

		vector<uint8_t> src1( width*height, 0 ), src2( width*height, 0 );
		vector<uint8_t> dst1( width*height/8, 0 ), dst2( width*height/8, 0 );

		cl::CommandQueue queue( task.GetContext(), task.GetConnectedDevices()[0] );
		cl::CommandQueue queue2( task.GetContext(), task.GetConnectedDevices()[0] );

		cl::Buffer devSrc1( task.GetContext(), CL_MEM_READ_ONLY, width*height ), devDst1( task.GetContext(), CL_MEM_WRITE_ONLY, width*height/8 );
		cl::Buffer devSrc2( task.GetContext(), CL_MEM_READ_ONLY, width*height ), devDst2( task.GetContext(), CL_MEM_WRITE_ONLY, width*height/8 );

		const int LOCAL_WIDTH = 64, LOCAL_HEIGHT = 4;
		cl::NDRange localws( LOCAL_WIDTH, LOCAL_HEIGHT );
		cl::NDRange globalws( 
			LOCAL_WIDTH * (int)ceil( (double)(right-left+1) / (double)LOCAL_WIDTH ), 
			LOCAL_HEIGHT * (int)ceil( (double)(bottom-top+1) / (double)LOCAL_HEIGHT )
		);

		FillUpPitchThresholdArg( kernelPitch0, devSrc1, devDst1, width, height, left, top, right, bottom, pitch, 10, LOCAL_WIDTH*LOCAL_HEIGHT );
		FillUpPitchThresholdArg( kernelPitch1, devSrc2, devDst2, width, height, left, top, right, bottom, pitch, 10, LOCAL_WIDTH*LOCAL_HEIGHT );


		cl::UserEvent kickOff( task.GetContext() );
		cl::Event e0, e1, e2;
		cl::Event f0, f1, f2;

		vector<cl::Event> vKickOff; vKickOff.push_back( kickOff );
		queue.enqueueWriteBuffer( devSrc1, CL_FALSE, 0, width*height, src1.data(), &vKickOff, &e0 );

		vector<cl::Event> fve0; fve0.push_back( e0 ); fve0.push_back( kickOff );
		queue2.enqueueWriteBuffer( devSrc2, CL_FALSE, 0, width*height, src2.data(), &fve0, &f0 );

		vector<cl::Event> ve0; ve0.push_back( e0 );
		queue.enqueueNDRangeKernel( kernelPitch0, cl::NullRange, globalws, localws, &ve0, &e1 );

		vector<cl::Event> ve1; ve1.push_back( e1 );
		queue.enqueueReadBuffer( devDst1, CL_FALSE, 0, width*height/8, dst1.data(), &ve1, &e2  );

/*
		vector<cl::Event> vf0; vf0.push_back( f0 ); vf0.push_back( e1 );
		queue.enqueueNDRangeKernel( kernelPitch1, cl::NullRange, globalws, localws, &vf0, &f1 );

		vector<cl::Event> vf1; vf1.push_back( f1 ); vf1.push_back( e2 );
		queue.enqueueReadBuffer( devDst2, CL_FALSE, 0, width*height/8, dst2.data(), &vf1, &f2  );

		kickOff.setStatus( CL_COMPLETE );
		f2.wait();
*/
		kickOff.setStatus( CL_COMPLETE );
		e2.wait();

	}
	catch( cl::Error e ) {
		cout << e.what() << ": Error Code " << e.err() << endl;
	}
	catch( WorkbenchException e ) 
	{
		cout << e.what() << endl;
		cout << task.GetBuildLog() << endl;
	}
}


 
BOOST_AUTO_TEST_SUITE_END()