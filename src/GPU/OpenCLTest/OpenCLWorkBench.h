#ifndef __OPENCL_WORKBENCH_HPP__
#define __OPENCL_WORKBENCH_HPP__

#include <CL/cl.hpp>
#include <CL/cl.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace OpenCLWorkBench {

typedef std::runtime_error WorkbenchException;

//
// usage :
//   Workbench task;
//   task.ConnectDevice( "GT 640" )
//       .BuildContext()
//	     .BuildProgram( "OpenCLKernels.cl" );
//
class Workbench
{
public:
	Workbench();
	Workbench& ConnectDevice( const std::string& deviceName );
	Workbench& ConnectDevice( cl_device_type type );
	Workbench& BuildContext();
	Workbench& BuildProgram( const std::string& sourceFile, const std::string& compilerOptions = std::string("") );
	Workbench& DumpProgram( const std::string& filename );

	std::vector<cl::Device>& GetConnectedDevices();
	cl::Context& GetContext();
	cl::Program& GetProgram();
	std::string GetBuildLog();

private:
    class Impl;
    boost::shared_ptr<Impl> _pimpl;
};

}

#endif