#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include "Common\Exception.hpp"

using namespace std;
using namespace ProgrammingStudy;

BOOST_AUTO_TEST_SUITE( TestDesignPatternFactory )

// interfaces
namespace Devices  {

class IGrabber{
public:
	virtual ~IGrabber() {}
	virtual int Grab( int frameCount ) = 0;
};
typedef boost::shared_ptr<IGrabber> IGrabberPtr;


enum GrabberType {
	GrabberPseudo = 0,
	GrabberCoco,
};

class IGrabberFactory {
public:
	virtual ~IGrabberFactory() {}
	virtual IGrabberPtr CreateGrabber( GrabberType gt ) = 0;
};
typedef boost::shared_ptr<IGrabberFactory> IGrabberFactoryPtr; 

}


// implementations
namespace Devices {

namespace Peudo {

class GrabberFromFile : public IGrabber {
public:
	GrabberFromFile( const _TCHAR* file ) {}

	int Grab( int frameCount )  {
		// grab using file
		return 1;
	}
};

}

namespace CoCo
{

class GrabberCoco : public IGrabber {
public:
	GrabberCoco( int deviceId ) {}
	int Grab( int frameCount )  {
		// grab using coco grabber
		return 2;
	}
};

}

class GrabberFactory : public IGrabberFactory {
public:
	IGrabberPtr CreateGrabber( GrabberType gt )
	{
		if( gt == GrabberPseudo ) 
		{
			return IGrabberPtr( new Peudo::GrabberFromFile( _T("somefile.bmp")) );
		}
		else if( gt == GrabberCoco ) 
		{
			return IGrabberPtr( new CoCo::GrabberCoco( 2 ) );
		}
		else
		{
			throw Exception( _T("Not supported type") );
		}
	}
};

}

BOOST_AUTO_TEST_CASE( ShouldFactory )
{  
	using namespace Devices;

	IGrabberFactoryPtr f( new GrabberFactory() );
	
	{
		IGrabberPtr g = f->CreateGrabber(GrabberPseudo);
		int r = g->Grab( 10 );
		BOOST_CHECK_EQUAL( r, 1 );
	}
	
	{
		IGrabberPtr g = f->CreateGrabber(GrabberCoco);
		int r = g->Grab( 10 );
		BOOST_CHECK_EQUAL( r, 2 );
	}
}



BOOST_AUTO_TEST_SUITE_END()