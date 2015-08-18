#include "StdAfx.h"

#include <stdexcept>
#include <stdint.h>
#include "Common/Exception.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>  
#include <boost/thread/shared_mutex.hpp>  
#include <boost/chrono.hpp>
#include <list>
#include <algorithm>


using namespace std;
using namespace ProgrammingStudy;
using namespace boost;

BOOST_AUTO_TEST_SUITE( TestDesignPatternObserver )



namespace Pattern {

class IEventArg {
public:
	virtual ~IEventArg() {}
};
typedef boost::shared_ptr<IEventArg> IEventArgPtr;

class IObserver {
public:
	virtual ~IObserver() {}
	virtual void Notify( void* object, IEventArgPtr arg ) = 0;
};
typedef boost::shared_ptr<IObserver> IObserverPtr;

class ISubject {
public:
	virtual ~ISubject() {}
	virtual void Register( IObserverPtr o ) = 0;
	virtual void Unregister( IObserverPtr o ) = 0;
	virtual void Fire( void* object, IEventArgPtr arg ) = 0;
};
typedef boost::shared_ptr<ISubject> ISubjectPtr;

}



namespace Pattern {

class DefaultSubject : public ISubject {
private:
	typedef std::list<IObserverPtr> ObserverCollection;
	ObserverCollection m_Observers;

public:
	void Register( IObserverPtr ob ) { m_Observers.push_back( ob ); }
	void Unregister( IObserverPtr ob ) { m_Observers.remove( ob ); }
	void Fire( void* object, IEventArgPtr arg ) {
		for( ObserverCollection::iterator I=m_Observers.begin(); I!=m_Observers.end(); ++I ) {
			IObserverPtr ob( *I );
			ob->Notify( object, arg );
		}
	}
};

}

namespace Grabber {

class IGrabber{
public:
	virtual ~IGrabber() {}
	virtual Pattern::ISubjectPtr GetSubject() = 0;
	virtual void Grab( int frameCount ) = 0;
	virtual void WaitForEnd() = 0;
};
typedef boost::shared_ptr<IGrabber> IGrabberPtr;

class IFrameReceivedEventArg : public Pattern::IEventArg {
public:
	virtual uint8_t* GetImageData() = 0;
	virtual int GetImageSize() const = 0;
};
typedef boost::shared_ptr<IFrameReceivedEventArg> IFrameReceivedEventArgPtr;

}

namespace Grabber {

class FrameReceivedEventArg : public IFrameReceivedEventArg {
private:
	uint8_t* m_Image; 
	int m_ImageSize;

public:
	FrameReceivedEventArg( uint8_t* image, int size ) : m_Image( image ), m_ImageSize( size ) {}
	uint8_t* GetImageData() { return m_Image; }	
	int GetImageSize() const { return m_ImageSize; }
};

class FooGrabber : public IGrabber {
private:
	int m_Interval;
	std::auto_ptr<boost::thread> m_Thread;
	Pattern::ISubjectPtr m_Subject;

public:
	FooGrabber() : m_Subject( new Pattern::DefaultSubject() ) {}
	Pattern::ISubjectPtr GetSubject() { return m_Subject; }

	struct FooGrabPredicate {
		FooGrabber& FG;
		int Count;
		FooGrabPredicate( FooGrabber& fg, int count) : FG( fg ), Count(count) {}
		void operator()() {
			IFrameReceivedEventArgPtr arg( new FrameReceivedEventArg( NULL, 0 ) );

			for( int i = 0; i<Count; ++i ) {
				this_thread::sleep_for( chrono::milliseconds(FG.m_Interval));
				FG.m_Subject->Fire( &FG, arg );
			}
		}
	};

	void Grab( int frameCount )  {
		FooGrabPredicate f( *this, frameCount );
		m_Thread = auto_ptr<boost::thread>( new boost::thread( f ) );
	}

	void WaitForEnd() { m_Thread->join(); }
};

}


namespace IPS  {

using namespace Pattern;
using namespace Grabber;

class Inspector {
private:
	struct GrabHandler : public IObserver {
		Inspector& This;
		GrabHandler( Inspector& t ) : This( t ) {}
		void Notify( void* object, IEventArgPtr arg ) {
			IFrameReceivedEventArg* fr = dynamic_cast<IFrameReceivedEventArg*>(arg.get());
			if( fr != NULL ) {
				This.Inspect( fr->GetImageData(), fr->GetImageSize());
			} else {
				throw Exception( "Not correct type" );
			}
		}
	};

	IObserverPtr m_GrabHandler;
	int m_InspectCount;

public:
	Inspector() : m_InspectCount(0) {}
	void Init() { m_GrabHandler = IObserverPtr( new GrabHandler( *this ) ); }
	void Inspect( uint8_t* image, int imageSize ) {
		m_InspectCount++;
	}
	IObserverPtr GetGrabHandler() { return m_GrabHandler; }
	int GetInspectCount() const { return m_InspectCount; }
};
typedef boost::shared_ptr<Inspector> InspectorPtr;

}

BOOST_AUTO_TEST_CASE( TestObserverPattern )
{  
	using namespace IPS;
	using namespace Grabber;

	IGrabberPtr foo( new FooGrabber() ); 
	InspectorPtr inspector( new Inspector() );
	inspector->Init();
	
	foo->GetSubject()->Register( inspector->GetGrabHandler() );

	foo->Grab( 10 );
	foo->WaitForEnd();

	BOOST_CHECK_EQUAL( inspector->GetInspectCount(), 10 );
}


namespace IPS  {

using namespace Pattern;
using namespace Grabber;

class Logging {
private:
	struct LoggingHandler : public IObserver {
		Logging& This;
		LoggingHandler( Logging& t ) : This( t ) {}
		void Notify( void* object, IEventArgPtr arg ) {
			IFrameReceivedEventArg* fr = dynamic_cast<IFrameReceivedEventArg*>(arg.get());
			This.Log( fr->GetImageData(), fr->GetImageSize());
		}
	};

	IObserverPtr m_GrabHandler;
	int m_LoggingCount;

public:
	Logging() : m_LoggingCount(0) {}
	void Init() { m_GrabHandler = IObserverPtr( new LoggingHandler( *this ) ); }
	void Log( uint8_t* image, int imageSize ) {
		m_LoggingCount++;
	}
	IObserverPtr GetGrabHandler() { return m_GrabHandler; }
	int GetLoggingCount() const { return m_LoggingCount; }
};

typedef boost::shared_ptr<Logging> LoggingPtr;
}

BOOST_AUTO_TEST_CASE( TestTwoObservers )
{  
	using namespace IPS;
	using namespace Grabber;

	IGrabberPtr foo( new FooGrabber() ); 

	InspectorPtr inspector( new Inspector() );
	inspector->Init();	
	foo->GetSubject()->Register( inspector->GetGrabHandler() );

	LoggingPtr logging( new Logging() );
	logging->Init();
	foo->GetSubject()->Register( logging->GetGrabHandler() );


	foo->Grab( 10 );
	foo->WaitForEnd();

	BOOST_CHECK_EQUAL( inspector->GetInspectCount(), 10 );
	BOOST_CHECK_EQUAL( logging->GetLoggingCount(), 10 );
}


#if 0

#endif

BOOST_AUTO_TEST_SUITE_END()