#ifndef __I_FOO_BAR_H__
#define __I_FOO_BAR_H__

#include "PlugIn\IPlugIn.h"
#include <boost\shared_ptr.hpp>

namespace ProgrammingStudy {
namespace PlugIn {

	
class IFoo : public IPlugIn
{
public:
	virtual int Foo( int i ) = 0;
};

typedef boost::shared_ptr<IFoo> IFooPtr;



class IBar : public IPlugIn
{
public:
	virtual int Bar( int i, int j ) = 0;
};

typedef boost::shared_ptr<IBar> IBarPtr;


class ILoo 
{
public:
	virtual ~ILoo() {}
	virtual int Loo( int i, int j ) = 0;
};

typedef boost::shared_ptr<ILoo> ILooPtr;

}
}

#endif