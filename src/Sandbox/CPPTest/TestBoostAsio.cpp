#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "Common\Exception.hpp"
#include "Common\Synchronization.hpp"

using namespace std;
using namespace boost;
using namespace ProgrammingStudy;

BOOST_AUTO_TEST_SUITE( TestBoostAsio )

class Wheel {
private:
	boost::asio::io_service m_IO;
	bool m_QuitNow;

public:
	Wheel() : m_QuitNow(false) {}
	boost::asio::io_service& GetIO() { return m_IO; }
	void operator() () { 
		for( ;!m_QuitNow; Sleep(10)) {
			m_IO.poll();
		}
	}
	void PostQuit() { m_QuitNow = true; }
};

typedef boost::shared_ptr<Wheel> WheelPtr;

BOOST_AUTO_TEST_CASE( TestSimple ) 
{  
	WheelPtr wheel( new Wheel() );
	boost::thread wheelThread( boost::ref(*wheel) );

	struct AsyncPrint {
		_tstring Msg;
		AsyncPrint( const _tstring& msg ) : Msg( msg ) {}
		void operator() () { _tprintf( Msg.c_str() ); }
	};
	wheel->GetIO().post( AsyncPrint(_T("Hello World")) );

	int myCount = 0;
	struct AsyncCount {
		int& Count;
		AsyncCount( int& count ) : Count(count) {}
		void operator() () { Count++; }
	};
	wheel->GetIO().post( AsyncCount(myCount) );

	Sleep(10);

	wheel->PostQuit();
	wheelThread.join();

	BOOST_CHECK_EQUAL( myCount, 1 );
}


class WorkerThread {
private:
	boost::asio::io_service m_IO;
	std::auto_ptr<boost::thread> m_Thread;
	double m_PollingPeriod;
	Event m_Quit;

protected:
	enum Running { KeepRunning = 0, StopRunning = 1 };
	virtual Running BackgroundWork() { return KeepRunning; }
	
public:
	WorkerThread( double pollingPeriod = 0.001 ) : 
		m_Quit( false, false ), m_PollingPeriod( pollingPeriod ) 
	{
		m_Thread = std::auto_ptr<boost::thread>( 
			new boost::thread( boost::ref( *this )));
	}

	boost::asio::io_service& GetIO() { return m_IO; }

	void operator() () { 

		for( ; ; ) {
			m_IO.poll();
			
			if( BackgroundWork() == StopRunning ) 
				break;

			if( m_Quit.TryWaitFor( m_PollingPeriod ) == true )
				break;
		}
	}

	bool TryWaitForEnd( double second) { 
		m_Quit.Set();
		return m_Thread->try_join_for( boost::chrono::milliseconds( (int)(second*1000) )); 
	}

};

typedef boost::shared_ptr<WorkerThread> WorkerThreadPtr;

BOOST_AUTO_TEST_CASE( TestWorkerThread ) 
{  
	WorkerThread worker;

	struct AsyncPrint {
		_tstring Msg;
		AsyncPrint( const _tstring& msg ) : Msg( msg ) {}
		void operator() () { _tprintf( Msg.c_str() ); }
	};
	worker.GetIO().post( AsyncPrint(_T("Hello World")) );

	int myCount = 0;
	struct AsyncCount {
		int& Count;
		AsyncCount( int& count ) : Count(count) {}
		void operator() () { Count++; }
	};
	worker.GetIO().post( AsyncCount(myCount) );

	worker.TryWaitForEnd( 1 );
	BOOST_CHECK_EQUAL( myCount, 1 );
}

class MessageLoopThread : public WorkerThread {
private: 
	int m_MessageLoopCount;

protected:
	Running BackgroundWork() { 
		m_MessageLoopCount++;
		return KeepRunning;
	}

public:
	MessageLoopThread() : m_MessageLoopCount(0) {}
	int GetMessageLoopCount() const { return m_MessageLoopCount; }
};

BOOST_AUTO_TEST_CASE( TestMessageLoop ) 
{  
	MessageLoopThread loop;

	struct AsyncPrint {
		_tstring Msg;
		AsyncPrint( const _tstring& msg ) : Msg( msg ) {}
		void operator() () { _tprintf( Msg.c_str() ); }
	};
	loop.GetIO().post( AsyncPrint(_T("Hello World")) );

	int myCount = 0;
	struct AsyncCount {
		int& Count;
		AsyncCount( int& count ) : Count(count) {}
		void operator() () { Count++; }
	};
	loop.GetIO().post( AsyncCount(myCount) );

	loop.TryWaitForEnd( 1 );
	BOOST_CHECK_EQUAL( myCount, 1 );
	BOOST_CHECK( loop.GetMessageLoopCount() > 0 );
}


BOOST_AUTO_TEST_SUITE_END()