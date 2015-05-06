#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestPointers )

BOOST_AUTO_TEST_CASE( ShouldPointToSameAddress )
{  
	int i = 1234;
	int *j = &i;

	BOOST_CHECK_EQUAL( i, *j );
}

BOOST_AUTO_TEST_CASE( TestPointerInArray )
{  
	int i[10] = { 0, 11, 22, 33, 44, 55, 66, 77, 88, 99 };
	int *j = i;

	BOOST_CHECK_EQUAL( *j, 0 );

	j++;
	BOOST_CHECK_EQUAL( *j, 11 );

	j += 3;
	BOOST_CHECK_EQUAL( *j, 44 );

	*j = -100;
	BOOST_CHECK_EQUAL( i[4], -100 );
}

BOOST_AUTO_TEST_CASE( TestNonCompatiblePointer )
{  
	int i[10] = { 0, 11, 22, 33, 44, 55, 66, 77, 88, 99 };

#if 0
	char *j = i;   // compiler error : incompatible pointer
#endif

	char *j = (char*)i;
	*j = -100;		// don't even bother to guess what i[0] will be ! 
}

BOOST_AUTO_TEST_CASE( TestPointerPointer ) 
{  
	int a[10] = { 0, 11, 22, 33, 44, 55, 66, 77, 88, 99 };
	int b[10] = { -1, -11, -22, -33, -44, -55, -66, -77, -88, -99 };
	int *c[2] = { a, b };
	int **d =  c;

	BOOST_CHECK_EQUAL( *(d+0), a );
	BOOST_CHECK_EQUAL( *(*(d+0)), a[0] );
	BOOST_CHECK_EQUAL( **d, a[0] );
	
	BOOST_CHECK_EQUAL( *(*(d)+1), a[1] );

	BOOST_CHECK_EQUAL( *(d+1), b );
	BOOST_CHECK_EQUAL( *(*(d+1)+1), b[1] );
}

BOOST_AUTO_TEST_SUITE_END()