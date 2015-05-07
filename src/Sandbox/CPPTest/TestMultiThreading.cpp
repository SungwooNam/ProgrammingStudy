#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>  
#include <boost/thread/shared_mutex.hpp>  
#include <boost/thread/thread.hpp>  
#include <boost/chrono.hpp>
#include <windows.h>

using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( TestMultiThreading )


BOOST_AUTO_TEST_CASE( UsingWin32CriticalSection ) 
{  
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	{
		EnterCriticalSection( &cs );

		// ...
		// do some data manipulation and hope no exception thrown
		// ...

		LeaveCriticalSection( &cs );
	}

	DeleteCriticalSection(&cs);
}




class CriticalSeciton
{
public:
	CriticalSeciton() { InitializeCriticalSection(&m_cs); }
	~CriticalSeciton() { DeleteCriticalSection(&m_cs); }
	void lock() { EnterCriticalSection( &m_cs ); }
	void unlock() { LeaveCriticalSection( &m_cs ); }

private:
	CRITICAL_SECTION m_cs;

	CriticalSeciton(CriticalSeciton const&);
    CriticalSeciton& operator=(CriticalSeciton const&);
};

BOOST_AUTO_TEST_CASE( UsingCriticalSection ) 
{  
	CriticalSeciton lockFifo;

	{
		boost::lock_guard<CriticalSeciton> guard( lockFifo );

		// ...
		// do some data manipulation and 
		// no worry about release the guard 
		// as it will be called automatically when it goes out of scope
		// ...	}
	}

	typedef boost::lock_guard<CriticalSeciton> GuardCS;
	{
		GuardCS guard( lockFifo );
		
		// ...
	}
}



BOOST_AUTO_TEST_CASE( UsingBoostMutex ) 
{  
	typedef boost::shared_mutex Mutex;
	typedef boost::lock_guard<Mutex> GuardMutex;

	Mutex lockFifo;
	{
		GuardMutex guard( lockFifo );

		// ...
	}
}



BOOST_AUTO_TEST_CASE( UsingReadAndWriteLock ) 
{  
	typedef boost::shared_mutex Mutex;
    typedef boost::shared_lock<Mutex> ReadLock;
    typedef boost::unique_lock<Mutex> WriteLock;
    Mutex mutex;

    {
        ReadLock read( mutex );

    }

    {
        WriteLock write( mutex, boost::adopt_lock_t() );

    }
}


struct InspectionRoutine {
	InspectionRoutine( int param ) : Param(param) {}
    void operator()() {
		Param++;
	}

	int Param;
};

BOOST_AUTO_TEST_CASE( TestBoostThreadUsingClass ) 
{	
	InspectionRoutine r1(1);
	boost::thread t1( r1  );

	InspectionRoutine r2(2);
	boost::thread t2( boost::ref(r2) );

	t1.join();
	t2.join();

	BOOST_CHECK_EQUAL( r1.Param, 1 );
	BOOST_CHECK_EQUAL( r2.Param, 3 );
}


void InspectionFunction( int* count )
{
	(*count) = (*count) + 1;
}

BOOST_AUTO_TEST_CASE( TestBoostThreadUsingFunction ) 
{	
	int count = 1;
	boost::thread t1( InspectionFunction, &count );
	boost::thread t2( InspectionFunction, &count );

	t1.join();
	t2.join();

	BOOST_CHECK_EQUAL( count, 3 );
}



typedef boost::shared_mutex Mutex;
typedef boost::lock_guard<Mutex> GuardMutex;

class CountingUp {
public:
	CountingUp( Mutex& lock, int& count, int times ) : m_Lock(lock), m_Count(count), m_Times(times) {}
    void operator()() {
		while( m_Times-- > 0 ) {
			this_thread::sleep_for( chrono::milliseconds(  m_Count%3 ));

			{
				GuardMutex guard( m_Lock );
				m_Count++;
			}
		}
	}

private:
	Mutex& m_Lock;
	int& m_Count;
	int m_Times;
};


BOOST_AUTO_TEST_CASE( TestThreadAndMutex ) 
{	
	Mutex lock;
	int count = 0;

	CountingUp c1( lock, count, 5 ), c2( lock, count, 2 ), c3( lock, count, 3 );
	boost::thread t1( boost::ref(c1) ), t2( boost::ref(c2) ), t3( boost::ref(c3) );

	t1.join(); t2.join(); t3.join();
	BOOST_CHECK_EQUAL( count, 10 );
}





BOOST_AUTO_TEST_SUITE_END() 