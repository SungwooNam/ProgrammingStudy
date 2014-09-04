#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestFloatingPoint )

BOOST_AUTO_TEST_CASE( TestBasic )
{  
	float number = 1.25;

	unsigned int rep = *(unsigned int *)(&number);

	bool is_positive = ( rep & 0x80000000 ) == 0 ? true : false;
	unsigned int mantissa = (rep & 0x007FFFFF);
	unsigned int exponent = (rep & 0x7F800000) >> 23;

	double sum = 1.0;
	double sub = 1.0 / 2.0;
	for( int n=22; n>=0; --n )
	{
		int bit = ((mantissa>>n)&0x01);
		double part = (double) bit * sub;
		sum += part;
		sub /= 2.0;
	}

	sum = sum * pow( 2.0, (double)exponent - 0x7f );
}


BOOST_AUTO_TEST_SUITE_END()