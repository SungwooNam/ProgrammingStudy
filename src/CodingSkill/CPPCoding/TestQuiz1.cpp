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


int nesting_1( const string &S ) 
{
    stack<char> q;
    
    string::const_iterator I;
    for( I=S.begin(); I!=S.end(); ++I )
    {
        const char& c( *I );
        if ( c == '(' ) 
        {
            q.push( c );
        }
        else if ( c== ')' )
        {
            if( q.size() == 0 ) 
            {
                return 0;
            }

            q.pop();
        }
    }

    if ( q.size() > 0 ) 
        return 0;

    return 1;
}

int nesting( const string &S ) 
{
    int q = 0;
    
    string::const_iterator I;
    for( I=S.begin(); I!=S.end(); ++I )
    {
        const char& c( *I );
        if ( c == '(' ) 
        {
            ++q;
        }
        else if ( c== ')' )
        {
            if( q <= 0 ) 
            {
                return 0;
            }

            --q;
        }
    }

    if ( q > 0 ) 
        return 0;

    return 1;
}

BOOST_AUTO_TEST_CASE( TestRun )
{
    BOOST_CHECK_EQUAL( 1, nesting( "(()())" ) );
    BOOST_CHECK_EQUAL( 0, nesting( "(()()" ) );
}


int dominator_o1 ( const vector<int> &A ) 
{
    map<int,int> o;
    for( size_t i=0; i<A.size();++i )
    {
        o[ A[i] ]++;
    }

    struct greatest {
        bool operator() ( const pair<int,int>& i, const pair<int,int>& j) 
        { 
            return i<j; 
        }
    };

    map<int,int>::iterator I =
        max_element( o.begin(), o.end(), greatest() );
    if( I == o.end() )
    {
        return -1;
    }

    int dom_i = (*I).first;
    int dom_count = (*I).second;
    if( dom_count < (int)(A.size()/2) ) 
        return -1;

    for( size_t i=0; i<A.size();++i )
    {
        if( A[i] == dom_i )
            return i;
    }

    return -1;
}

int dominator_o2 ( const vector<int> &A ) 
{
    map<int,int> o;
    for( size_t i=0; i<A.size();++i )
    {
        o[ A[i] ]++;
    }

    int dom_i = -1;
    map<int,int>::iterator I;
    for( I = o.begin(); I!= o.end(); ++I )
    {
        int dom_count = (*I).second;
        if( dom_count >= (int)(A.size()/2) ) 
        {
            dom_i = (*I).first;
            break;
        }
    }

    if( dom_i == -1 ) 
        return -1;

    for( size_t i=0; i<A.size();++i )
    {
        if( A[i] == dom_i )
            return i;
    }

    return -1;
}

int dominator ( const vector<int> &A ) 
{
    map<int,pair<int,int>> o;
    map<int,pair<int,int>>::iterator I;
    for( size_t i=0; i<A.size();++i )
    {
        I = o.find( A[i] );
        if( I == o.end() ) 
        {
            pair<map<int,pair<int,int>>::iterator,bool> P = o.insert( make_pair(A[i],make_pair(0,i)) );
            I = P.first;
        }
        (*I).second.first ++;
    }

    for( I = o.begin(); I!= o.end(); ++I )
    {
        int dom_count = (*I).second.first;
        if( dom_count >= (int)(A.size()/2) ) 
        {
            return (*I).second.second;
        }
    }

    return -1;
}


BOOST_AUTO_TEST_CASE( TestRun2 )
{
    {
        int a[] = { 1, 2, 3, 3, 3, 3 };
        vector<int> A( a, a + sizeof(a)/sizeof(a[0]) );
        BOOST_CHECK_EQUAL( 2, dominator( A ) );
    }
    {
        BOOST_CHECK_EQUAL( -1, dominator( vector<int>() ) );
    }
}

double weight( int a )
{
    char buf[255];
    int len = sprintf( buf, "%d", a );

    int sum = 0;
    for( int i =0; i<len; ++i )
    {
        sum += (buf[i]-'0');
    }

    return (double)sum / len;
}

int heavy_decimal_count1 ( int A,int B ) 
{   
    int c = 0;
    for( int v=A; v<=B; ++v )
    {
        if( weight(v) > 7.0 )
        {
            c++;
        }
    }

    return c;
}

int weight_sum( int a, int& len, int& last )
{
    char buf[255];
    len = sprintf( buf, "%d", a );

    int sum = 0;
    for( int i=0; i<len; ++i )
    {
        last = (buf[i]-'0');
        sum += last;
    }

    return sum;
}

int heavy_decimal_count ( int A,int B ) 
{   
    int c = 0;
    int len, sum, last;
    int check = 0;
    for( int v=A; v<=B; ++v )
    {
        if( check == 0 ) 
        {
            sum = weight_sum( v, len, last );
            if( last == 0 ) 
            {
                check = 9;
            }
        }
        else
        {
            sum ++;
            check--;
        }

        if( (double)sum/len > 7.0 )
        {
            c++;
        }
    }

    return c;
}


BOOST_AUTO_TEST_CASE( TestRun3 )
{
    {
        BOOST_CHECK_EQUAL( 5, heavy_decimal_count( 8675, 8691 ) );
    }
}


int binomial_coeff( int n, int k )
{
    if ( k < 0 || k > n ) 
        return 0;

    if ( k == 0 ) return 1;
    
    return binomial_coeff( n-1, k-1 ) + binomial_coeff( n-1,k );
}

class MemoizedBinomial
{
public:
	int Coeff( short n, short k )
	{
		if ( k < 0 || k > n ) 
			return 0;

		if ( k == 0 ) return 1;

		int key = ( n<<16 | k );
		std::map<int,int>::iterator I = m_Cache.find( key );
		if( I != m_Cache.end() ) 
		{
			return I->second;
		}

		int value = Coeff( n-1, k-1 ) + Coeff( n-1, k );
		m_Cache[key] = value;
		return value;
	}
	
private:
	std::map<int,int> m_Cache;

};

int count_decimal_up_to( int d, int n, MemoizedBinomial& binomial )
{
    int mi = -1;
    int sum = 0;
    for( int i=0; i<=d; ++i )
    {
        mi *= -1;
		sum += mi* binomial.Coeff(d,i) * binomial.Coeff( n + d - 10*i, d );
    }
    return sum;
}


BOOST_AUTO_TEST_CASE( TestRun4 )
{
	{
		MemoizedBinomial bm;
		BOOST_CHECK_EQUAL( 10, bm.Coeff( 5, 2 ) );
	}

    {
        BOOST_CHECK_EQUAL( 10, binomial_coeff( 5, 2 ) );
    }

    {
		MemoizedBinomial binomial;

        BOOST_CHECK_EQUAL( 6, count_decimal_up_to( 1, 5, binomial ) );
        BOOST_CHECK_EQUAL( heavy_decimal_count(100,199), 100 - count_decimal_up_to( 2, 21 - 1, binomial ) );
        BOOST_CHECK_EQUAL( heavy_decimal_count(1000,1999), 1000 - count_decimal_up_to( 3, 28 - 1, binomial ) );
    }
}