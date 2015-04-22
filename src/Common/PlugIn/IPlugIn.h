#ifndef __I_PLUG_IN_H__
#define __I_PLUG_IN_H__

#include <string>
#include <vector>

namespace ProgrammingStudy {
namespace PlugIn {

class IPlugIn
{
public:
	virtual ~IPlugIn() {};
	virtual const char * GetID() const = 0;
};

typedef std::vector<std::string> PlugInNameArray;

class IPlugInFactory : public IPlugIn
{
public:
	virtual PlugInNameArray GetPlugInIDs() const = 0;
	virtual IPlugIn* Create( const char *ID ) = 0;
};

}
}

#endif