#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/locks.hpp>  
#include <boost/thread/shared_mutex.hpp>  

using namespace std;

BOOST_AUTO_TEST_SUITE( TestMultiThreading )


// code to test do things like critical section

// code to do critical section using Win32


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

// code to do above test using thread




BOOST_AUTO_TEST_SUITE_END() 