#include "StdAfx.h"

#include <stdexcept>
#include <stdint.h>
#include "Common/Exception.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>  
#include <boost/thread/shared_mutex.hpp>  
#include <boost/chrono.hpp>


using namespace std;
using namespace ProgrammingStudy;
using namespace boost;

BOOST_AUTO_TEST_SUITE( TestDesignPatternCallback )

// interfaces
namespace Devices  {

class IGrabberCallbck {
public:
	virtual ~IGrabberCallbck() {}
	virtual void OnReceived( int frameNum, uint8_t* image, int size ) = 0;
};
typedef boost::shared_ptr<IGrabberCallbck> IGrabberCallbackPtr;

class IGrabber{
public:
	virtual ~IGrabber() {}
	virtual void SetCallback( IGrabberCallbackPtr callback ) = 0;
	virtual void Grab( int frameCount ) = 0;
	virtual void WaitForEnd() = 0;
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
	GrabberFromFile( int interval ) : m_Interval(interval) {}
	void SetCallback( IGrabberCallbackPtr callback ) { m_Callback = callback; }

	struct PesudoGrabPredicate {
		GrabberFromFile& GFF;
		int Count;
		PesudoGrabPredicate( GrabberFromFile& gff, int count) : GFF( gff ), Count(count) {}
		void operator()() {
			for( int i = 0; i<Count; ++i ) {
				this_thread::sleep_for( chrono::milliseconds(GFF.m_Interval));
				GFF.m_Callback->OnReceived( i, NULL, 0 );
			}
		}
	};

	void Grab( int frameCount )  {
		PesudoGrabPredicate f( *this, frameCount );
		m_Thread = auto_ptr<boost::thread>( new boost::thread( f ) );
	}

	void WaitForEnd() { m_Thread->join(); }

private:
	IGrabberCallbackPtr m_Callback;
	int m_Interval;
	auto_ptr<boost::thread> m_Thread;
};

}


class GrabberFactory : public IGrabberFactory {
public:
	IGrabberPtr CreateGrabber( GrabberType gt )	{
		if( gt == GrabberPseudo ) {
			return IGrabberPtr( new Peudo::GrabberFromFile(5) );
		}

		throw Exception( _T("Not supported type") );
	}
};

}


namespace IPS  {

struct Inspector : public Devices::IGrabberCallbck {
	int Count;

	Inspector() : Count(0) {}
	void OnReceived( int frameNum, uint8_t* image, int size ) { Count++; }
};

}

BOOST_AUTO_TEST_CASE( ShouldCallMyCallback )
{  
	using namespace Devices;
	using namespace IPS;

	IGrabberFactoryPtr f( new GrabberFactory() );
	IGrabberCallbackPtr inspector( new Inspector() );
	
	{
		IGrabberPtr g = f->CreateGrabber(GrabberPseudo);
		g->SetCallback( inspector );
		g->Grab( 10 );
		g->WaitForEnd();

		Inspector* insp = (Inspector*)(inspector.get());
		BOOST_CHECK_EQUAL( insp->Count, 10 );
	}
}


#if 0
Inspector* insp = dynamic_cast<Inspector*>(inspector.get());


typedef boost::shared_ptr<Inspector> InspectorPtr;

BOOST_AUTO_TEST_CASE( TestSmartPointerPolymorphism )
{  
	using namespace Devices;
	using namespace IPS;

	IGrabberFactoryPtr f( new GrabberFactory() );
	InspectorPtr inspector( new Inspector() );
	
	{
		IGrabberPtr g = f->CreateGrabber(GrabberPseudo);
		g->SetCallback( inspector );
		g->Grab( 10 );
		g->WaitForEnd();

		BOOST_CHECK_EQUAL( inspector->Count, 10 );
	}
}

#endif

BOOST_AUTO_TEST_SUITE_END()