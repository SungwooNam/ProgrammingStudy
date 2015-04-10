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

namespace SumToZeroThreeItem
{

class SumToZeroFinder
{
public:
	bool IsAddToZero( const int A[], int sizeA, int sum, int level ) const 
	{
		if( level <= 3 )
		{
			for( int i=0; i<sizeA; ++i )
			{
				if( IsAddToZero( A, sizeA, sum + A[i], level+1 ) ) 
					return true;
			}
			return false;
		}

		return sum == 0;
	}

	bool IsAddZeroWithSet( const int A[], int sizeA ) const 
	{
		set<int> S( A, A+sizeA );

		for( int i=0; i<sizeA; ++i)
		{
			for( int j=0; j<sizeA; ++j)
			{
				int Ak = -(A[i] + A[j]);

				if( S.find( Ak ) != S.end() )
					return true;
			}
		}

		return false;
	}
};


}


BOOST_AUTO_TEST_CASE( TestSumToZeroThreeItemByRecursive )
{
	using namespace SumToZeroThreeItem;

	SumToZeroFinder finder;

	{
		int A[] = { 1, 2, -3, 10 };
		bool isZero = finder.IsAddToZero(  A, 4, 0, 1 );
		BOOST_CHECK_EQUAL( isZero, true );
	}

	{
		int A[] = { 10, 2, -1 };
		bool isZero = finder.IsAddToZero(  A, 4, 0, 1 );
		BOOST_CHECK_EQUAL( isZero, true );
	}

	{
		int A[] = { 10, 2, -3};
		bool isZero = finder.IsAddToZero(  A, 4, 0, 1 );
		BOOST_CHECK_EQUAL( isZero, false );
	}
}


BOOST_AUTO_TEST_CASE( TestSumToZeroThreeItemBySet )
{
	using namespace SumToZeroThreeItem;

	SumToZeroFinder finder;

	{
		int A[] = { 1, 2, -3, 10 }; 
		bool isZero = finder.IsAddZeroWithSet( A, 4 );
		BOOST_CHECK_EQUAL( isZero, true );
	}

	{
		int A[] = { 10, 2, -1 };
		bool isZero = finder.IsAddZeroWithSet(  A, 4 );
		BOOST_CHECK_EQUAL( isZero, true );
	}

	{
		int A[] = { 10, 2, -3};
		bool isZero = finder.IsAddZeroWithSet(  A, 4);
		BOOST_CHECK_EQUAL( isZero, false );
	}
}

