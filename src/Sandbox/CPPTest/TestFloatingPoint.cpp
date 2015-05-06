#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestFloatingPoint )

BOOST_AUTO_TEST_CASE( ShouldNotEqualWhen1Over3 )
{  
	float a1 = (float)( 1.0/3.0 );
	double a2 = 1.0/3.0;

	BOOST_CHECK( (double)a1 != a2 );
}

BOOST_AUTO_TEST_CASE( TestPointer )
{  
	float a = 1.0f;
	float* b = &a;

	BOOST_CHECK_EQUAL( a, *b);
}


BOOST_AUTO_TEST_SUITE_END()