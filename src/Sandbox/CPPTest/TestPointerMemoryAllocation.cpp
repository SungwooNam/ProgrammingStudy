#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestPointerMemoryAllocation )

BOOST_AUTO_TEST_CASE( TestNewDelete ) 
{  
	char *name = new char[100];
	strcpy_s( name, 100, "TestNewDelete" );
	BOOST_CHECK_EQUAL( name, "TestNewDelete" );

	char *another_name = name;
	BOOST_CHECK_EQUAL( another_name, "TestNewDelete" );

	delete name;  // memory leak if we delete this !

	// strcpy_s( another_name, 100, "NO NO NO" );	// access violation !
	// delete another_name;							// access violation !
}


BOOST_AUTO_TEST_CASE( TestFreeMemoryLeak ) 
{  
	try
	{
		char *name = new char[100];
		strcpy_s( name, 100, "TestFreeMemoryLeak" );
		
		// throw "Got some error!"; // memory leak if enable this line

		delete name;  
	}
	catch( ... )
	{

	}
}


BOOST_AUTO_TEST_SUITE_END() 