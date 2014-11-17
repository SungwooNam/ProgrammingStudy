#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>
#include <limits>
#include <set>
#include <math.h>



using namespace std;

int FindMid( vector<int>& A )
{
    if ( A.size() == 0 ) 
        return -1;

    long long rsum = std::accumulate( A.begin()+1, A.end(), 0 );
    long long lsum = 0;

    if( lsum == rsum ) 
        return 0;

    for( size_t i=1; i<A.size(); ++i )
    {
        lsum += A[i-1];
        rsum -= A[i];

        if ( rsum == lsum ) 
            return i;
    }

    return -1;
}


BOOST_AUTO_TEST_CASE( TestCodilityDemo )
{
    {
        int A[] = { 1, -2, 1 };
        BOOST_CHECK_EQUAL( 1, FindMid( vector<int>( A, A+3 ) ) );
    }

    {
        int A[] = { 1, 2, -2, 1, 1 };
        BOOST_CHECK_EQUAL( 3, FindMid( vector<int>( A, A+(sizeof(A)/sizeof(A[0])) ) ) );
    }
}


int ClosestAscender( int A[], int length, int k )
{
    if( k < 0 || k >= length ) 
        return -1;

    vector<int> as;
    for( int i=0; i<length; ++i )
    {
        if( i == k ) continue;

        if( A[i] > A[k] ) 
            as.push_back( i );
    }

    int min = numeric_limits<int>::max();
    int minIndex = -1;
    for( int j = 0; j < (int)as.size(); ++j) 
    {
        int dist = abs( as[j]-k );
        if( dist < min ) 
        {
            min = dist;
            minIndex = as[j];
        }
    }

    return minIndex;
}

BOOST_AUTO_TEST_CASE( TestClosestAscender )
{
    {
        int A[] = { 1, 2, 3, 4, 5 };
        BOOST_CHECK_EQUAL( 3, ClosestAscender( A, sizeof(A)/sizeof(A[0]), 2 ) );
    }

    {
        int A[] = { 1, 5, 3, 4, 4 };
        BOOST_CHECK_EQUAL( 1, ClosestAscender( A, sizeof(A)/sizeof(A[0]), 4 ) );
    }
}


long long Fib( int N )
{
    if( N >= 2 ) 
    {
        return Fib( N-1 ) + Fib( N -2 );
    }
    
    return N;
}

long long FibImp( int N )
{
    long long a = 0;
    long long b = 1;
    long long t;

    for( int i = 0; i<N; ++i )
    {
        t = a;
        a = b;
        b = b + t;
    }
    return a;
}

int power_fib( int N, int M )
{
    return FibImp( pow( (double)N, (double)M ) ) % 10000103;
}



BOOST_AUTO_TEST_CASE( TestPowerFib )
{
    BOOST_CHECK_EQUAL( 21, power_fib( 2, 3 ) );
}


int FirstCoveringPrefix( int A[], int length )
{   
    if ( length == 0 ) 
        return -1;

    set<int> keys;
    int prefix = -1;

    for( int i = 0; i<length; ++i )
    {
        pair<set<int>::iterator,bool> ret = keys.insert ( A[i] );
        if( ret.second == true )
        {
            prefix = i;
        }
    }

    return prefix;
}

BOOST_AUTO_TEST_CASE( TestFirstCoveringPrefix )
{
    {
        int A[] = { 1, 5, 3, 4, 4 };
        BOOST_CHECK_EQUAL( 3, FirstCoveringPrefix( A, sizeof(A)/sizeof(A[0]) ) );
    }

    {
        int A[] = { 1 };
        BOOST_CHECK_EQUAL( 0, FirstCoveringPrefix( A, sizeof(A)/sizeof(A[0]) ) );
    }    

    {
        int A[] = { 1, 1, 1, 1, 1 };
        BOOST_CHECK_EQUAL( 0, FirstCoveringPrefix( A, sizeof(A)/sizeof(A[0]) ) );
    }    

    {
        int A[] = { 1 };
        BOOST_CHECK_EQUAL( -1, FirstCoveringPrefix( A, 0 ) );
    }  

    {
        int A[] = { 1,2,3,4,5 };
        BOOST_CHECK_EQUAL( 4, FirstCoveringPrefix( A, sizeof(A)/sizeof(A[0]) ) );
    }  

}

