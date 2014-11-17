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
#include <unordered_map>

using namespace std;
using namespace boost;

// go through each element and check whether it is incremented and return range

struct Range
{
	int Start;
	int End;

	Range( int s, int e ) : Start( s), End(e) {}
	Range( int s) : Start( s), End(s) {}
};

typedef boost::shared_ptr<Range> RangePtr;

class ConsecutiveNumbers
{
public:
	list< RangePtr> FindConsecutives( int numbers[], int numbersCount )
	{
		list<RangePtr> ranges;

		if( numbersCount == 0 ) 
			return ranges;

		int index = 0;
		RangePtr r( new Range( numbers[index++] ) );

		for( ; index<numbersCount; index++ )
		{
			if( r->End >= numbers[index] )
			{
				throw "Should have numbers in ascending order";
			}

			if( r->End+1 == numbers[index] )
			{
				r->End = numbers[index]; 
			}
			else
			{
				ranges.push_back( r );

				r = RangePtr( new Range( numbers[index] ) );
			}
		}

		ranges.push_back( r );
		return ranges;
	}
};

BOOST_AUTO_TEST_CASE( TestQuiz5 )
{
	int numbers[] = { 4, 5, 6, 7, 8, 9, 12, 15, 16, 17, 18, 20, 22, 23, 24, 27 };

	ConsecutiveNumbers finder;
	list<RangePtr> ranges = 
		finder.FindConsecutives( numbers, sizeof( numbers ) / sizeof( numbers[0] ) );


}

