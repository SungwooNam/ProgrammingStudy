#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>
#include <limits>
#include <list>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <math.h>

using namespace std;


__int64 CollatzProblem()
{
    map<__int64,__int64> seq;
    map<__int64,__int64>::iterator I;
    seq[2] = 1;

    for( int i=3; i< (int)1e6; ++i )
    {
        stack<__int64> series;
        __int64 v = i;

        for( ;; )
        {
            I = seq.find( v );
            if( I == seq.end() ) 
            {
                series.push( v );
                v = ( v%2 == 0 ) ? ( v/2 ) : ( 3*v+1 );
                if ( v < 0 ) 
                {
                    break;
                }
            }
            else
            {
                __int64 c = I->second + 1;
                while( series.size() > 0 ) 
                {   
                    seq[ series.top() ] = c;
                    series.pop();
                    c++;
                }

                break;
            }
        }
    }

    struct CountComp {
        bool operator()( const pair<__int64,__int64>& l, const pair<__int64,__int64>& r) { return l.second<r.second; }
    };

    I = max_element( seq.begin(), seq.end(), CountComp() );
    return I->first;
}

__int64 CollatzProblem_UsingLinearAddress( int N )
{
    int* seq = new int[ N ];
    memset( seq, 0, sizeof(int)*N );
    seq[2] = 1;

    for( int i=3; i< N; ++i )
    {
        stack<__int64> series;
        __int64 v = i;

        for( ;; )
        {
            if( v>=N || (v<N && seq[ (int)v ] == 0) )
            {
                series.push( v );
                v = ( v%2 == 0 ) ? ( v/2 ) : ( 3*v+1 );
                if ( v < 0 ) 
                {
                    break;
                }
            }
            else
            {
                int c = seq[ (int)v ] + 1;
                while( series.size() > 0 ) 
                {   
                    if ( series.top() < N ) {
                        seq[ (int)series.top() ] = c;
                    }
                    series.pop();
                    c++;
                }

                break;
            }
        }
    }

    struct CountComp {
        bool operator()( const int& l, const int& r) { return l<r; }
    };

    int ret = max_element( seq, seq+N, CountComp() ) - seq;
    delete seq;

    return ret;
}

BOOST_AUTO_TEST_CASE( TestEulerProject14 )
{
    //BOOST_CHECK_EQUAL( 837799, CollatzProblem() );
    //BOOST_CHECK_EQUAL( 837799, CollatzProblem_UsingLinearAddress( 1000000) );
}


void PathInGrid( int r, int d, __int64 &count )
{
    if ( r == 0 || d == 0 ) 
    {
        count++;
        return;
    }

    PathInGrid( r-1, d, count );
    PathInGrid( r, d-1, count );
}

__int64 PathInGrid_Fast( int d )
{
    vector<__int64> P, C;
    
    P.assign( 3, 0 );
    P[2]=6; P[1]=3;

    for( int n = 3; n <= d; ++n )
    {
        C.assign( n+1, 0 );

        C[1] = n + 1;
        C[n] = 1 + P[1];
        for( int i=2; i<n; ++i) 
        {
            C[i] = P[i] + C[i-1];
            C[n] += P[i];
        }
        C[n] *= 2;

        P.swap( C );
    }

    return P[ d ];
}


BOOST_AUTO_TEST_CASE( TestEulerProject15 )
{
    {
        __int64 count = 0;
        PathInGrid( 3, 3, count );
        BOOST_CHECK_EQUAL( 20, count );
    }

    {
        BOOST_CHECK_EQUAL( 20, PathInGrid_Fast( 3) );
        //BOOST_CHECK_EQUAL( 137846528820, PathInGrid_Fast( 20 ) );
    }
}


__int64 DigitSumOfTwoSquare( int power )
{
	class Number
	{
	public:
        Number( const Number& r ) : _V( r._V ) {}

        Number( const char *str )
		{
			const char *l = str + strlen( str );
			while ( l != str ) 
			{
				_V.push_back( *(--l) - '0' );
			}
		}

		void Add( const Number& r )
		{
			char v = 0;
			size_t i=0;
			for( ; i<r._V.size(); ++i )
			{
				if( i >= _V.size() ) 
				{
					_V.push_back( r._V[i] );
				}

				v += r._V[i] + _V[i]; 
				_V[i] = v%10;
				v /=  10;
			}

			while( v != 0 )
			{
				if( i >= _V.size() ) {
					_V.push_back( v );
					break;
				}
				else
				{
					v += _V[i]; 
					_V[i] = v % 10;
					v /=  10;
				}
				++i;
			}
		}

		string AsString() const 
		{
			string r;
			for( int i=_V.size()-1; i>=0; --i)
			{
				r.push_back( _V[i] + '0' );
			}

			return r;
		}

	private:
		vector<char> _V;
	};

	if( power == 0 ) 
		return 1;

	Number v( "2" );
	for( int i = 2; i<=power; ++i ) 
	{
		Number a( v );
		v.Add( a );
	}

    struct IntCharAdd {
        __int64 operator() ( __int64 s, const char& v ) { return s + ( v - '0' ); }
    };

    string vs( v.AsString() );
    return accumulate( vs.begin(), vs.end(), (__int64)0, IntCharAdd() );
}

BOOST_AUTO_TEST_CASE( TestEulerProject16 )
{
    BOOST_CHECK_EQUAL( 26, DigitSumOfTwoSquare( 15 ) );
//    BOOST_CHECK_EQUAL( 1366, DigitSumOfTwoSquare( 1000 ) );
}

int LetterCountSumFrom0neTo( int n )
{
    class NumericWord
    {
    public:
        static int Count( int v )
        {
            static int letters_0_19[] = 
            { 
                0,
                strlen("one"),
                strlen("two"),
                strlen("three"),
                strlen("four"),
                strlen("five"),
                strlen("six"),
                strlen("seven"),
                strlen("eight"),
                strlen("nine"),
                strlen("ten"),
                strlen("eleven"),
                strlen("twelve"),
                strlen("thirteen"),
                strlen("fourteen"),
                strlen("fifteen"),
                strlen("sixteen"),
                strlen("seventeen"),
                strlen("eighteen"),
                strlen("nineteen"),
            };

            static int letters_0_90[] = 
            {
                0,
                0,
                strlen("twenty"),
                strlen("thirty"),
                strlen("forty"),
                strlen("fifty"),
                strlen("sixty"),
                strlen("seventy"),
                strlen("eighty"),
                strlen("ninety"),
            };

            int c1 = v / 20;
            if ( c1 == 0 )
            {
                return letters_0_19[ v % 20 ];
            }
            else if ( v < 100 )
            {
                return letters_0_90[ v / 10 ] + letters_0_19[ v % 10 ];
            }
            else if ( v < 1000 ) 
            {
                static int hundred = 7;
                static int and = 3;

                if( v%100 == 0 )
                {
                    return letters_0_19[ v / 100 ] + hundred;
                }
                
                return letters_0_19[ v / 100 ] + hundred + and + Count( v % 100 );
            }
            else if ( v == 1000 )
            {
                return 11;
            }

            return -1;
        }

        static int SummationOfCount( int s, int e )
        {
            int sum = 0;
            for( int i = s; i<=e; ++i ) { sum += Count(i); }
            return sum;
        }

        static int SummationOfCount( int n ) // FIX ME : This method is not working!
        {
            if( n <= 19 ) 
            {
                return SummationOfCount( 1, n );
            }
            else if ( n < 100 )
            {
                int sum = SummationOfCount( 1, 19 );
                int s_9 = SummationOfCount( 0, 9 );

                int i = 20;
                for( ; i< (n/10)*10; i+= 10 )
                {
                    sum += Count(i) * 10;
                    sum += s_9;
                }

                sum += Count(i) * ((n%10)+1);
                sum += SummationOfCount( 1, n % 10 );
                return sum;
            }
            else if ( n < 1000 )
            {
                static int hundred = 7;
                static int and = 3;
                int s99 = SummationOfCount( 99 );
                int i = 100, sum = 0;
                for( ; i < (n/100)*100; i+=100 )
                {
                    sum += hundred;
                    sum += ( Count( i / 100 ) + + hundred + and ) * 100;
                    sum += s99;
                }
                
                sum += hundred;
                sum += ( Count( i / 100 ) + hundred + and ) * ( n % 100 );
                sum += SummationOfCount( 1, n%100 );
                return sum;
            }
            else if ( n == 1000 ) 
            {
                static int one_thousand = 11;
                int s999 = SummationOfCount( 999 );
                return s999 + one_thousand;
            }

            return -1;
        }
    };

    return NumericWord::SummationOfCount( 1, n );
}

BOOST_AUTO_TEST_CASE( TestEulerProject17 )
{
    BOOST_CHECK_EQUAL( 19, LetterCountSumFrom0neTo( 5 ) );
//    BOOST_CHECK_EQUAL( 21124, LetterCountSumFrom0neTo( 1000 ) );
}

struct Place {
    int Row, Col;
    int Sum, UpperBound;
    Place() {};
    Place( int row, int col, int sum ) : Row(row), Col(col), Sum(sum)
    {
        UpperBound = sum + (s_RowCount - row - 1)*s_MaxValue;
    }

    bool operator() ( const Place& l, const Place& r ) 
    {
        if( l.UpperBound == r.UpperBound ) {
            if( l.Sum == r.Sum ) {
                if( l.Row == r.Row ) 
                    return l.Col > r.Col;
                return l.Row > r.Row;
            }
            return l.Sum > r.Sum;
        }
        return l.UpperBound > r.UpperBound;
    }

    static void Dump( const Place & p )
    {
        printf( "r=%d,c=%d,s=%d,u=%d\n", p.Row, p.Col, p.Sum, p.UpperBound );
    }

    static void SetUpperBoundVariable( int rowCount, int maxValue )
    {
        s_RowCount = rowCount;
        s_MaxValue = maxValue;
    }

    static int s_RowCount, s_MaxValue;
};

int Place::s_RowCount = 0;
int Place::s_MaxValue = 0;

int FindMaxTopToBottom( const vector< vector<int> >& triangle, int maxValue )
{
    typedef priority_queue< Place, vector<Place>, Place > PlaceQueue;

    int n = triangle.size();
    Place::SetUpperBoundVariable( n, maxValue );

    PlaceQueue q;
    q.push( Place( 0,0, triangle[0][0] ));
    int max = 0, iter = 0;

    while( q.size() > 0 ) 
    {
        Place t( q.top() );
        q.pop();
        Place::Dump( t );

        if( t.Row < n-1 )
        {
            if( t.UpperBound < max ) 
                continue;

            q.push( Place( t.Row+1, t.Col, t.Sum + triangle[t.Row+1][t.Col] ) );
            q.push( Place( t.Row+1, t.Col+1, t.Sum + triangle[t.Row+1][t.Col+1] ) );
        }
        else
        {
            if( t.Sum > max ) 
            {
                max = t.Sum;
            }   
        }

        iter++;
    }
       
    printf( "max = %d, iter count = %d\n", max, iter );
    return max;
}

vector<int> ParseAsVector( const char *numbers )
{
    vector<int> v;
    const char *s = numbers;
    char *e = const_cast<char *>(numbers);
    while( *e != NULL ) 
    {
        int i = strtol( s, &e, 10  );
        if( i == 0 )
            break;

        v.push_back( i );
        s = e;
    }
    return v;
}

int FindMaxTopToBottomFromBottomToTop( vector< vector<int> >& t )
{
    for( size_t i=t.size()-1; i>0; --i )
    {
        for( size_t j=0; j<t[i].size()-1; ++j )
        {
            t[i-1][j] += max( t[i][j], t[i][j+1] );
        }
    }
       
    return t[0][0];
}


BOOST_AUTO_TEST_CASE( TestEulerProject18 )
{
    {
        vector< vector<int> > triangle;
        triangle.assign( 4, vector<int>() );
        triangle[0].swap( ParseAsVector( "3" ) );
        triangle[1].swap( ParseAsVector( "7 4" ) );
        triangle[2].swap( ParseAsVector( "2 4 6" ) );
        triangle[3].swap( ParseAsVector( "8 5 9 3" ) );

        int ret = FindMaxTopToBottom( triangle, 9 );
        BOOST_CHECK_EQUAL( 23, ret );
    }

    {
        vector< vector<int> > triangle;
        triangle.assign( 15, vector<int>() );
        triangle[0].swap( ParseAsVector( "75" ) );
        triangle[1].swap( ParseAsVector( "95 64" ) );
        triangle[2].swap( ParseAsVector( "17 47 82" ) );
        triangle[3].swap( ParseAsVector( "18 35 87 10" ) );
        triangle[4].swap( ParseAsVector( "20 04 82 47 65" ) );
        triangle[5].swap( ParseAsVector( "19 01 23 75 03 34" ) );
        triangle[6].swap( ParseAsVector( "88 02 77 73 07 63 67" ) );
        triangle[7].swap( ParseAsVector( "99 65 04 28 06 16 70 92" ) );
        triangle[8].swap( ParseAsVector( "41 41 26 56 83 40 80 70 33" ) );
        triangle[9].swap( ParseAsVector( "41 48 72 33 47 32 37 16 94 29" ) );
        triangle[10].swap( ParseAsVector( "53 71 44 65 25 43 91 52 97 51 14" ) );
        triangle[11].swap( ParseAsVector( "70 11 33 28 77 73 17 78 39 68 17 57" ) );
        triangle[12].swap( ParseAsVector( "91 71 52 38 17 14 91 43 58 50 27 29 48" ) );
        triangle[13].swap( ParseAsVector( "63 66 04 68 89 53 67 30 73 16 69 87 40 31" ) );
        triangle[14].swap( ParseAsVector( "04 62 98 27 23 09 70 98 73 93 38 53 60 04 23" ) );

//        BOOST_CHECK_EQUAL( 1074, FindMaxTopToBottom( triangle, 99 ) );
    }
}

BOOST_AUTO_TEST_CASE( TestEulerProject67 )
{
#if 0
    {
        vector< vector<int> > triangle;
        triangle.clear();

        {
            FILE *fp = fopen( "triangle.txt", "rt" );
    
            while( !feof( fp ) ) {
                char buf[1024];
                if( fgets( buf, 1024, fp ) == NULL )
                    break;

                triangle.push_back( ParseAsVector( buf ) );
            }

            fclose( fp );
        }

        int ret = FindMaxTopToBottomFromBottomToTop( triangle);
        BOOST_CHECK_EQUAL( 7273, ret );
    }
#endif
}
