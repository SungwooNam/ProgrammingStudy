#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>
#include <limits>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <math.h>
#include <stdio.h>

using namespace std;

namespace InplaceReverseWord
{

void inplace_reverse( char *arr, int length )
{
	for( int i=0; i<length/2; ++i)
	{
		char temp = arr[ i ];
		arr[i] = arr[ length -1 -i ];
		arr[ length -1 -i ] = temp;
	}
}

void inplace_reverse_word( char *arr, int length )
{
	inplace_reverse( arr, length );

	int start = 0;
	for( int i=0; i<length; ++i)
	{
		if( arr[i] == ' ' )
		{
			inplace_reverse( arr+start, i-start );

			++i;
			start = i;
		}
	}
}


std::string inplace_reverse_word( const char* sentence )
{
	int length = strlen(sentence);
	std::vector<char> s( length );
	copy( sentence, sentence+length, s.begin() );

	inplace_reverse_word( &s[0], length );

	return std::string( s.begin(), s.end() );
}


}


BOOST_AUTO_TEST_CASE( TestInplaceReverseWord )
{
	using namespace InplaceReverseWord;

    {
        BOOST_CHECK_EQUAL( 
			"Christmas Merry a you wish I", 
			inplace_reverse_word( "I wish you a Merry Christmas").c_str() );
    }
}
