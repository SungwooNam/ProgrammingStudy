#include <boost/test/unit_test.hpp>
#include <vector>
#include <numeric>
#include <limits>
#include <list>
#include <set>
#include <map>
#include <math.h>

using namespace std;

int findSumOfTwoMod( int ubound, int m1, int m2 )
{
	int n1 = ubound / m1;
	int n2 = ubound / m2;
	int nc = ubound / ( m1*m2);
	return (m1*n1*(n1+1)/2) + ( m2*n2*(n2+1)/2) - (m1*m2*nc*(nc+1)/2);
}

BOOST_AUTO_TEST_CASE( TestEulerProject1 )
{
	BOOST_CHECK_EQUAL( 23, findSumOfTwoMod( 9, 3, 5 ) );
	BOOST_CHECK_EQUAL( 233168, findSumOfTwoMod( 999, 3, 5 ) );
}


long long findEvenValuedFinoacciSum( long long max )
{
    long long a = 0;
    long long b = 1;
    long long t;
    long long sum = 0;

    while( true ) 
    {
        t = a;
        a = b;
        b = b + t;

        if( b > max ) 
            break;

        if( b % 2 == 0 ) {
            sum += b;
        }
    }

    return sum;
}

BOOST_AUTO_TEST_CASE( TestEulerProject2 )
{
	long long sum = findEvenValuedFinoacciSum( (long long )4e6 );
}

long long findHighestPrimeFactor( long long N )
{
    vector<long long> primes;
    for( long long i = 2; i < N; ++i ) 
    {
        bool is_prime = true;
        for( size_t j = 0; j<primes.size(); ++j )
        {
            if( i % primes[j] == 0 ) 
            {
                is_prime = false;
                break;
            }
        }

        if ( is_prime ) 
        {
            primes.push_back(i);

            while( N%i == 0 )
            {
                N = N / i;
            }
        }

        if( i*2 > N ) 
            break;
    }

    return N;
}

long long findHighestPrimeFactor_Opt( long long N )
{
    long long f = 2L;
    while( N % f == 0 )
    {
        N = N / f;
    }

    if ( N == 1L ) { 
        return f;
    }

    f = 3L;
    long long sqrtN = (long long) sqrt( (double)N );
    while( true ) 
    {
        while( N % f == 0 )
        {
            N = N / f;
            sqrtN = (long long)sqrt( (double)N );
        }

        if( f > sqrtN ) 
            break;

        f += 2L;
    }

    return N;
}

BOOST_AUTO_TEST_CASE( TestEulerProject3 )
{
	BOOST_CHECK_EQUAL( 29L, findHighestPrimeFactor_Opt( 13195L ) );
    BOOST_CHECK_EQUAL( 6857L, findHighestPrimeFactor_Opt( 600851475143L ));
}

int ProductOf3DigitsFromHighest( int nth )
{
    if( nth == 0 )
        return 999 * 999;
    else if( nth == 1 )
        return 999 * 998;

    int n = 998 - ( (nth-2) / 4 );
    int step = (nth-2) % 4;
    switch( step )
    {
    case 0:     return n*n;
    case 1:     return (n+1)*(n-1);
    case 2:     return n*(n-1);
    case 3:     return (n+1)*(n-2);
    }

    throw logic_error("shouldn't be here");
}

bool IsPalindrome( int N )
{
    char buf[255];
    sprintf( buf, "%d", N );

    char rev[255];
    strcpy( rev, buf );
    _strrev( rev );

    return strcmp( buf , rev ) == 0;
}

int LargetPalinedromeOf3DigitProduct() 
{
    set<int,greater<int> > products;
    for( int i = 999; i>=100; --i )
    {
        for( int j = i; j >=100; --j )
        {
            products.insert( i*j );
        }

        set<int,greater<int>>::iterator low = products.lower_bound( i*i );
        for( set<int,greater<int>>::iterator I = products.begin(); I!=low; ++I )
        {
            int num = *I;
            if( IsPalindrome( num ) )
                return num;
        }

        products.erase( products.begin(), low );
    }
     
    return -1;
}

BOOST_AUTO_TEST_CASE( TestEulerProject4 )
{
// too slow
//    BOOST_CHECK_EQUAL( 906609, LargetPalinedromeOf3DigitProduct() );
    
}


__int64 LeastEvenlyDivisible( __int64 N )
{
    map<__int64,int> pc;
    pc[ 2L ] = 1;

    for( __int64 i=1; i<=N; ++i )
    {
        __int64 ni = i;

        map<__int64,int>::iterator I;
        for( I = pc.begin(); I!= pc.end(); ++I )
        {
            pair<const __int64,int>& p( *I );
            __int64 prime = p.first;

            int c = 0;
            while( ni % prime == 0 )
            {
                ni = ni / prime;
                c++;
            }
            
            p.second = std::max( c, p.second );
        }

        if ( ni != 1 ) 
        {
            pc[ ni ] = 1;
        }
    }

    __int64 r = 1;
    map<__int64,int>::iterator I;
    for( I = pc.begin(); I!= pc.end(); ++I )
    {
        pair<const __int64,int>& p( *I );
        __int64 prime = p.first;
        int factor = p.second;
        r *= (__int64)pow( (long double)prime, factor );
    }

    return r;
}

vector<__int64> primes( __int64 N )
{
    vector<__int64> p; 

    if ( N >= 2 ) {
        p.push_back( 2 );
    }

    for( __int64 i=3; i<=N; i+=2 )
    {
        __int64 ni = i;
        size_t j;
        bool found = false;
        __int64 ul = (__int64)sqrt( (long double)ni );
        for( j=0; j<p.size(); ++j )
        {
            if( p[j] > ul ) {
                p.push_back( i );
                break;
            }
                
            if ( ni % p[j] == 0 )
            {
                break;
            }
        }
    }

    return p;
}

__int64 LeastEvenlyDivisible_Opt( __int64 N )
{
    vector<__int64> p( primes( N ) );

    __int64 r = 1;
    for( size_t i=0; i< p.size(); ++i )
    {
        int factor = (int) floor( log( (long double)N ) / log( (long double)p[i] ) );

        r *= (__int64) pow( (long double) p[i], factor );
    }

    return r;
}

BOOST_AUTO_TEST_CASE( TestEulerProject5 )
{
	BOOST_CHECK_EQUAL( 2520, LeastEvenlyDivisible( 10 ) );
    BOOST_CHECK_EQUAL( 232792560L, LeastEvenlyDivisible( 20 ) );

    BOOST_CHECK_EQUAL( 2520, LeastEvenlyDivisible_Opt( 10 ) );
    BOOST_CHECK_EQUAL( 232792560L, LeastEvenlyDivisible_Opt( 20 ) );

}


__int64 DiffSum( int N )
{
    __int64 ss = N * (N+1) / 2;
    __int64 ds = 0;
    for( int i=1; i<=N; ++i )
    {
        ds += i * ( ss - i );
    }

    return ds;
}

BOOST_AUTO_TEST_CASE( TestEulerProject6 )
{
	BOOST_CHECK_EQUAL( 2640L, DiffSum( 10 ) );
	BOOST_CHECK_EQUAL( 25164150L, DiffSum( 100 ) );
}


__int64 nth_prime( __int64 n )
{
    vector<__int64> p; 

    if ( n == 1 ) 
    {
        return 2L;
    }

    p.push_back( 2 );
    n--;

    __int64 i;
    for( i=3; i<=numeric_limits<__int64>::max(); i+=2 )    
    {
        size_t j=0;
        for( ; j<p.size(); ++j )
        {
            if ( i % p[j] == 0 )
                break;
        }
        
        if( j == p.size() )
        {
            p.push_back(i);
            if( (--n) <= 0 )
                break;
        }
    }

    return i;
}

BOOST_AUTO_TEST_CASE( TestEulerProject7 )
{
    BOOST_CHECK_EQUAL( 13L, nth_prime( 6 ) );
    //BOOST_CHECK_EQUAL( 104743, nth_prime( 10001L ) );     // too much time
}

int LargestProductOfConsequitiveDigits( const char *digits, int n )
{
    int length = strlen( digits );
    if ( length <= 0 ) 
        return 0;

    vector<int> d( length );
    for( int i=0; i<length; ++i )
    {
        d[i] = digits[i] - '0';
    }

    int maxp = 0;
    int maxd0 = -1;
    for( int i = 0; i<length-n; ++i )
    {
        if( d[i] >= maxd0  )
        {
            maxd0 = d[i];
            int p = d[i]*d[i+1]*d[i+2]*d[i+3]*d[i+4];
            if( p > maxp ) 
            {
                maxp = p;
            }
        }
    }

    return maxp;
}


int LargestProductOfConsequitiveDigits_Opt( const char *digits, int n )
{
    int length = strlen( digits );
    if ( length <= 0 ) 
        return 0;

    vector<int> d( length );
    for( int i=0; i<length; ++i )
    {
        d[i] = digits[i] - '0';
    }
   
    int cur, max;
    cur = max = accumulate(  &d[0], &d[n], 1, multiplies<int>());
    for( int i = 1; i<length-n; ++i )
    {
        if( cur != 0 ) 
        {
            cur /= d[i]; 
            cur *= d[i+n-1];
        }
        else
        {
            cur = accumulate( &d[i], &d[i+n], 1, multiplies<int>());
        }

        if ( cur > max ) 
        {
            max = cur;
        }
    }

    return max;
}

BOOST_AUTO_TEST_CASE( TestEulerProject8 )
{
    const char* digits = 
        "73167176531330624919225119674426574742355349194934"
        "96983520312774506326239578318016984801869478851843"
        "85861560789112949495459501737958331952853208805511"
        "12540698747158523863050715693290963295227443043557"
        "66896648950445244523161731856403098711121722383113"
        "62229893423380308135336276614282806444486645238749"
        "30358907296290491560440772390713810515859307960866"
        "70172427121883998797908792274921901699720888093776"
        "65727333001053367881220235421809751254540594752243"
        "52584907711670556013604839586446706324415722155397"
        "53697817977846174064955149290862569321978468622482"
        "83972241375657056057490261407972968652414535100474"
        "82166370484403199890008895243450658541227588666881"
        "16427171479924442928230863465674813919123162824586"
        "17866458359124566529476545682848912883142607690042"
        "24219022671055626321111109370544217506941658960408"
        "07198403850962455444362981230987879927244284909188"
        "84580156166097919133875499200524063689912560717606"
        "05886116467109405077541002256983155200055935729725"
        "71636269561882670428252483600823257530420752963450"
        ;

    BOOST_CHECK_EQUAL( 40824, LargestProductOfConsequitiveDigits( digits, 5 ) );
    BOOST_CHECK_EQUAL( 40824, LargestProductOfConsequitiveDigits_Opt( digits, 5 ) );
}

int PythagoreanTripletSumedTo1000()
{
    int a,b;
    a=b=0;

    for( b = 1; b < 500; ++b )
    {   
        if( (1000*(500-b) % ( 1000-b )) == 0 ) 
        {   
            a = (1000*(500-b) / ( 1000-b ));
            break;
        }
    }

    return a * b * ( 1000 - a - b );
}

BOOST_AUTO_TEST_CASE( TestEulerProject9 )
{
    BOOST_CHECK_EQUAL( 31875000, PythagoreanTripletSumedTo1000() );
}


#if 0
list<__int64> primes_by_sieves( __int64 N )
{
    char* s = new char[(__int64)N]; 
    memset( s, 1, N );
 
    for( __int64 i=4; i<=N; i+=2 )
    {
        s[i] = 0;
    }

    __int64 ul = (__int64)sqrt( (long double)N );
    for( __int64 i=3; i<=ul; i+=2 )
    {
        if( s[i] == 0 )
            continue;

        for( __int64 j=i*i; j<=N; j+=i )
        {
            s[j] = 0;
        }
    }

    list<__int64> p;
    for( __int64 i=2; i<=N; ++i )
    {
        if( s[i] == 1 ) {
            p.push_back( i );
        }
    }

    delete[] s;

    return p;
}
#endif

BOOST_AUTO_TEST_CASE( TestEulerProject10 )
{
#if 0
    vector<__int64> ps( primes( (__int64)2e6 ) );
    __int64 r = accumulate( ps.begin(), ps.end(), (__int64)0 );
    BOOST_CHECK_EQUAL( 142913828922, r );


    list<__int64> ps( primes_by_sieves( (__int64)2e6 ) );
    __int64 r = accumulate( ps.begin(), ps.end(), (__int64)0 );
    BOOST_CHECK_EQUAL( 142913828922, r );
#endif
}

int FindGreatestProductInAllDirection( int *A, int rows, int columns, int count )
{
    struct Matrix 
    { 
        Matrix( int *a, int cols ) { _A = a; _Cols = cols; }

        int *_A;
        int _Cols;

        int Value( int x, int y ) const
        {
            return _A[ y * _Cols + x ];
        }
    };

    struct LargestProduct
    {
        static int OfConsequitiveDigits( const vector<int>& d, int n )
        {
            int maxi=0;
            int cur, max;
            cur = max = accumulate(  &d[0], &d[n], 1, multiplies<int>());
            for( size_t i = 1; i<d.size()-n; ++i )
            {
                if( cur != 0 ) 
                {
                    cur /= d[i-1]; 
                    cur *= d[i+n-1];
                }
                else
                {
                    cur = accumulate( &d[i], &d[i+n], 1, multiplies<int>());
                }

                if ( cur > max ) 
                {
                    max = cur;
                    maxi = i;
                }
            }

            return max;
        }
    };

    Matrix a( A, columns );
    vector<int> va;

    {
        for( int y=0; y<rows; ++y) 
        {
            for( int x=0; x<columns; ++x )
            {
                va.push_back( a.Value( x,y) );
            }
            va.push_back( 0 );
        }
    }

    {
        for( int x=0; x<columns; ++x )
        {
            for( int y=0; y<rows; ++y) 
            {
                va.push_back( a.Value( x,y) );
            }
            va.push_back( 0 );
        }
    }

    {
        for( int x=count-1; x<columns*2; ++x )
        {
            for( int y=0; y<=rows; ++y) 
            {
                if( x-y >= columns ) { 
                    continue;
                }

                if( x-y < 0 || y>=rows ) {
                    va.push_back( 0 );
                    break;
                }
                va.push_back( a.Value( x-y,y) );
            }
        }
    }

    {
        for( int x=-columns; x<columns-count; ++x )
        {
            for( int y=0; y<=rows; ++y) 
            {
                if ( x+y < 0 ) 
                    continue;

                if( x+y >= columns || y>=rows ) {
                    va.push_back( 0 );
                    break;
                }
                va.push_back( a.Value( x+y,y) );
            }
        }
    }

    return LargestProduct::OfConsequitiveDigits( va, count );
}

BOOST_AUTO_TEST_CASE( TestEulerProject11 )
{
    int A[20][20] = {
        {  8, 2,22,97,38,15, 0,40, 0,75, 4, 5, 7,78,52,12,50,77,91, 8 },
        { 49,49,99,40,17,81,18,57,60,87,17,40,98,43,69,48, 4,56,62, 0 },
        { 81,49,31,73,55,79,14,29,93,71,40,67,53,88,30, 3,49,13,36,65 },
        { 52,70,95,23, 4,60,11,42,69,24,68,56, 1,32,56,71,37, 2,36,91 },
        { 22,31,16,71,51,67,63,89,41,92,36,54,22,40,40,28,66,33,13,80 },
        { 24,47,32,60,99, 3,45, 2,44,75,33,53,78,36,84,20,35,17,12,50 },
        { 32,98,81,28,64,23,67,10,26,38,40,67,59,54,70,66,18,38,64,70 },
        { 67,26,20,68,02,62,12,20,95,63,94,39,63, 8,40,91,66,49,94,21 },
        { 24,55,58, 5,66,73,99,26,97,17,78,78,96,83,14,88,34,89,63,72 },
        { 21,36,23, 9,75, 0,76,44,20,45,35,14, 0,61,33,97,34,31,33,95 },
        { 78,17,53,28,22,75,31,67,15,94,03,80,04,62,16,14, 9,53,56,92 },
        { 16,39, 5,42,96,35,31,47,55,58,88,24,00,17,54,24,36,29,85,57 },
        { 86,56, 0,48,35,71,89, 7, 5,44,44,37,44,60,21,58,51,54,17,58 },
        { 19,80,81,68, 5,94,47,69,28,73,92,13,86,52,17,77, 4,89,55,40 },
        {  4,52, 8,83,97,35,99,16, 7,97,57,32,16,26,26,79,33,27,98,66 },
        { 88,36,68,87,57,62,20,72, 3,46,33,67,46,55,12,32,63,93,53,69 },
        {  4,42,16,73,38,25,39,11,24,94,72,18, 8,46,29,32,40,62,76,36 },
        { 20,69,36,41,72,30,23,88,34,62,99,69,82,67,59,85,74, 4,36,16 },
        { 20,73,35,29,78,31,90, 1,74,31,49,71,48,86,81,16,23,57, 5,54 },
        {  1,70,54,71,83,51,54,69,16,92,33,48,61,43,52, 1,89,19,67,48 } };

    int max = FindGreatestProductInAllDirection( (int*)A, 20, 20, 4 );
    BOOST_CHECK_EQUAL( 70600674, max );
}

int DivisorCount( const map<int,int>& divisors )
{
    int p = 1;
    map<int,int>::const_iterator I;
    for( I = divisors.begin(); I!=divisors.end(); ++I )
    {
        p *= ((*I).second + 1);
    }
    return p;
}

map<int,int> Factors( int n, const vector<int>& primes )
{
    map<int,int> d;

    for( size_t i=0; i<primes.size(); ++i )
    {
        int p = primes[i];
        int c = 0;
        while( n % p == 0 )
        {
            n /= p;
            c++;
        }

        if( c >= 1 ) 
        {
            d[ p ] = c;
        }
    }

    if ( n != 1 ) 
    {
        d[ n ] = 1;
    }

    return d;
}

int FirstTriangleNumberOverDivisors( int N )
{
    struct Divisor
    {
        Divisor( int v ) : _v(v) {}
        bool operator() ( int d ) const {  return _v % d == 0;  }
        int _v;
    };

    vector<int> primes;
    primes.push_back( 2 );
    int upp = 3;

    for( int i = 5; i< numeric_limits<int>::max(); ++i )
    {
        int t = i * ( i+1 ) / 2;
        int st = (int)sqrt( (long double)t );

        int j = upp;
        for( ; j <= st; j+= 2 )
        {
            if( primes.end() == 
                find_if( primes.begin(), primes.end(), Divisor( j ) ) )
            {
                primes.push_back( j );
            }
        }

        upp = j; 

        int dc =  DivisorCount( Factors( t, primes )  );

        if( dc > N )
            return t;
    }

    return -1;
}


BOOST_AUTO_TEST_CASE( TestEulerProject12 )
{
    BOOST_CHECK_EQUAL( 28, FirstTriangleNumberOverDivisors( 5 ) );
    BOOST_CHECK_EQUAL( 76576500, FirstTriangleNumberOverDivisors( 500 ) );
}



string SumBigNumber( const char** s, int ns )
{
	class Number
	{
	public:
		Number( const char *str )
		{
			const char *l = str + strlen( str );
			while ( l != str ) 
			{
				_values.push_back( *(--l) - '0' );
			}
		}

		void Add( const Number& r )
		{
			size_t digits = _values.size();
			char value = 0;
			size_t i=0;
			for( ; i<r._values.size(); ++i )
			{
				if( i >= digits ) 
				{
					_values.push_back( r._values[i] );
				}

				value += r._values[i] + _values[i]; 
				_values[i] = value%10;
				value /=  10;
			}

			while( value != 0 )
			{
				if( i >= digits ) {
					_values.push_back( value );
					break;
				}
				else
				{
					value += _values[i]; 
					_values[i] = value % 10;
					value /=  10;
				}
				++i;
			}
		}

		string AsString() const 
		{
			string ret;
			for( int i=_values.size()-1; i>=0; --i)
			{
				ret.push_back( _values[i] + '0' );
			}

			return ret;
		}

	private:
		vector<char> _values;
	};

	if( ns <= 0 ) 
		return 0;

	Number sum( s[0] );
	for( int i = 1; i<ns; ++i ) 
	{
		Number n( s[i] );
		sum.Add( n );
	}

	return sum.AsString();
}


BOOST_AUTO_TEST_CASE( TestEulerProject13 )
{
	{
		const char* values[] = { 
			"9",
			"8",
			"9",
			"9",
		};

		BOOST_CHECK_EQUAL( "35", SumBigNumber( values, 4 ).c_str() );
	}

	{
		const char* values[] = { 
"37107287533902102798797998220837590246510135740250",
"46376937677490009712648124896970078050417018260538",
"74324986199524741059474233309513058123726617309629",
"91942213363574161572522430563301811072406154908250",
"23067588207539346171171980310421047513778063246676",
"89261670696623633820136378418383684178734361726757",
"28112879812849979408065481931592621691275889832738",
"44274228917432520321923589422876796487670272189318",
"47451445736001306439091167216856844588711603153276",
"70386486105843025439939619828917593665686757934951",
"62176457141856560629502157223196586755079324193331",
"64906352462741904929101432445813822663347944758178",
"92575867718337217661963751590579239728245598838407",
"58203565325359399008402633568948830189458628227828",
"80181199384826282014278194139940567587151170094390",
"35398664372827112653829987240784473053190104293586",
"86515506006295864861532075273371959191420517255829",
"71693888707715466499115593487603532921714970056938",
"54370070576826684624621495650076471787294438377604",
"53282654108756828443191190634694037855217779295145",
"36123272525000296071075082563815656710885258350721",
"45876576172410976447339110607218265236877223636045",
"17423706905851860660448207621209813287860733969412",
"81142660418086830619328460811191061556940512689692",
"51934325451728388641918047049293215058642563049483",
"62467221648435076201727918039944693004732956340691",
"15732444386908125794514089057706229429197107928209",
"55037687525678773091862540744969844508330393682126",
"18336384825330154686196124348767681297534375946515",
"80386287592878490201521685554828717201219257766954",
"78182833757993103614740356856449095527097864797581",
"16726320100436897842553539920931837441497806860984",
"48403098129077791799088218795327364475675590848030",
"87086987551392711854517078544161852424320693150332",
"59959406895756536782107074926966537676326235447210",
"69793950679652694742597709739166693763042633987085",
"41052684708299085211399427365734116182760315001271",
"65378607361501080857009149939512557028198746004375",
"35829035317434717326932123578154982629742552737307",
"94953759765105305946966067683156574377167401875275",
"88902802571733229619176668713819931811048770190271",
"25267680276078003013678680992525463401061632866526",
"36270218540497705585629946580636237993140746255962",
"24074486908231174977792365466257246923322810917141",
"91430288197103288597806669760892938638285025333403",
"34413065578016127815921815005561868836468420090470",
"23053081172816430487623791969842487255036638784583",
"11487696932154902810424020138335124462181441773470",
"63783299490636259666498587618221225225512486764533",
"67720186971698544312419572409913959008952310058822",
"95548255300263520781532296796249481641953868218774",
"76085327132285723110424803456124867697064507995236",
"37774242535411291684276865538926205024910326572967",
"23701913275725675285653248258265463092207058596522",
"29798860272258331913126375147341994889534765745501",
"18495701454879288984856827726077713721403798879715",
"38298203783031473527721580348144513491373226651381",
"34829543829199918180278916522431027392251122869539",
"40957953066405232632538044100059654939159879593635",
"29746152185502371307642255121183693803580388584903",
"41698116222072977186158236678424689157993532961922",
"62467957194401269043877107275048102390895523597457",
"23189706772547915061505504953922979530901129967519",
"86188088225875314529584099251203829009407770775672",
"11306739708304724483816533873502340845647058077308",
"82959174767140363198008187129011875491310547126581",
"97623331044818386269515456334926366572897563400500",
"42846280183517070527831839425882145521227251250327",
"55121603546981200581762165212827652751691296897789",
"32238195734329339946437501907836945765883352399886",
"75506164965184775180738168837861091527357929701337",
"62177842752192623401942399639168044983993173312731",
"32924185707147349566916674687634660915035914677504",
"99518671430235219628894890102423325116913619626622",
"73267460800591547471830798392868535206946944540724",
"76841822524674417161514036427982273348055556214818",
"97142617910342598647204516893989422179826088076852",
"87783646182799346313767754307809363333018982642090",
"10848802521674670883215120185883543223812876952786",
"71329612474782464538636993009049310363619763878039",
"62184073572399794223406235393808339651327408011116",
"66627891981488087797941876876144230030984490851411",
"60661826293682836764744779239180335110989069790714",
"85786944089552990653640447425576083659976645795096",
"66024396409905389607120198219976047599490197230297",
"64913982680032973156037120041377903785566085089252",
"16730939319872750275468906903707539413042652315011",
"94809377245048795150954100921645863754710598436791",
"78639167021187492431995700641917969777599028300699",
"15368713711936614952811305876380278410754449733078",
"40789923115535562561142322423255033685442488917353",
"44889911501440648020369068063960672322193204149535",
"41503128880339536053299340368006977710650566631954",
"81234880673210146739058568557934581403627822703280",
"82616570773948327592232845941706525094512325230608",
"22918802058777319719839450180888072429661980811197",
"77158542502016545090413245809786882778948721859617",
"72107838435069186155435662884062257473692284509516",
"20849603980134001723930671666823555245252804609722",
"53503534226472524250874054075591789781264330331690",
		};

//		BOOST_CHECK_EQUAL( "5537376230", SumBigNumber( values, 100 ).substr(0,10).c_str() );	
	}
}
