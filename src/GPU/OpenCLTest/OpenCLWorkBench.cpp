#include "OpenCLWorkBench.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <numeric>

using namespace std;

namespace OpenCLWorkBench {

class Workbench::Impl
{
public:
	Impl() {}

	void ConnectDevice( const string& deviceName )
	{
		vector<cl::Platform> platforms;	
		cl::Platform::get( &platforms );

		for( vector<cl::Platform>::iterator I=platforms.begin(); I!=platforms.end(); ++I )
		{
			cl::Platform& platform( *I );
			
			vector<cl::Device> devices;
			platform.getDevices( CL_DEVICE_TYPE_ALL, &devices );

			for( vector<cl::Device>::iterator J=devices.begin(); J!=devices.end(); ++J )
			{
				cl::Device& device( *J );
				string discoveredName( device.getInfo<CL_DEVICE_NAME>() );

				if( discoveredName.find( deviceName ) != discoveredName.npos )
				{
					_Devices.push_back( device );
					return;
				}
			}
		}

		throw WorkbenchException( string("Cannot find the device with the name") + deviceName );
	}

	void ConnectDevice( cl_device_type type )
	{
		vector<cl::Platform> platforms;	
		cl::Platform::get( &platforms );

		for( vector<cl::Platform>::iterator I=platforms.begin(); I!=platforms.end(); ++I )
		{
			cl::Platform& platform( *I );
			
			vector<cl::Device> devices;
			platform.getDevices( type, &devices );
			for( vector<cl::Device>::iterator J=devices.begin(); J!=devices.end(); ++J )
			{
				_Devices.push_back( *J );
			}
		}
	}

	void BuildContext()
	{
		_Context = cl::Context( GetConnectedDevices() );
	}

	void BuildProgram( const string& sourceFile, const string& options )
	{
		std::ifstream programFile( sourceFile.c_str() );
		std::string programString( 
			std::istreambuf_iterator<char>(programFile), 
			(std::istreambuf_iterator<char>()));
		cl::Program::Sources source( 
			1, 
			std::make_pair( programString.c_str(), programString.length()+1 )
			);

		_Program = cl::Program( _Context, source );
		_Program.build( GetConnectedDevices(), options.c_str() );
	}

	void DumpProgram( const string& filename )
	{
		const std::vector<size_t> binSizes = _Program.getInfo<CL_PROGRAM_BINARY_SIZES >();
		std::vector<unsigned char> binData( 
			std::accumulate( binSizes.begin(), binSizes.end(), 0 )
		);

		if( binData.size() == 0 ) 
		{
			throw WorkbenchException( "DumpProgram failied : Binary program is empty" );
		}

		unsigned char* binChunk = &binData[0];

		std::vector<unsigned char*> binaries;
		for( size_t i=0; i<binSizes.size(); ++i) {
			binaries.push_back( binChunk );
			binChunk += binSizes[i];
		}

		_Program.getInfo( CL_PROGRAM_BINARIES, &binaries[0] );

		std::ofstream binaryFile( filename.c_str() );
		if( ! binaryFile.good() ) 
		{
			throw WorkbenchException( "DumpProgram Failed : Failed to open binary file for kernel" );
		}
		
		for( size_t i=0; i<binaries.size(); ++i)
			binaryFile << binaries[i];
	}

	std::string GetBuildLog() 
	{
		if ( _Program() == NULL ) 
		{
			return std::string("");
		}
		return _Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>( GetConnectedDevices()[0] );
	}

	std::vector<cl::Device>& GetConnectedDevices() { return _Devices; }
	cl::Context& GetContext() { return _Context; }
	cl::Program& GetProgram() { return _Program; }
		
private:
	vector<cl::Device> _Devices;
	cl::Context _Context;
	cl::Program _Program;
};


Workbench::Workbench() : _pimpl( new Impl() ) {}
Workbench& Workbench::ConnectDevice( const string& deviceName ) { _pimpl->ConnectDevice( deviceName ); return *this; } 
Workbench& Workbench::ConnectDevice( cl_device_type type ) { _pimpl->ConnectDevice( type ); return *this; } 
Workbench& Workbench::BuildContext() { _pimpl->BuildContext(); return *this; }
Workbench& Workbench::BuildProgram( const string& sourceFile, const std::string& options ) { _pimpl->BuildProgram( sourceFile, options ); return *this; } 
Workbench& Workbench::DumpProgram( const string& filename ) { _pimpl->DumpProgram( filename ); return *this; }
std::vector<cl::Device>& Workbench::GetConnectedDevices() { return _pimpl->GetConnectedDevices(); }
cl::Context& Workbench::GetContext() { return _pimpl->GetContext(); }
cl::Program& Workbench::GetProgram() { return _pimpl->GetProgram(); }
std::string Workbench::GetBuildLog() { return _pimpl->GetBuildLog(); }

}

