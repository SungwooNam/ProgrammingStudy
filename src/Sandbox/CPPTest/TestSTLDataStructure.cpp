#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <math.h>
#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <set>
#include <map>
#include "Exception.hpp"
#include "StopWatch.hpp"

using namespace std;
using namespace boost;
using namespace ProgrammingStudy;

BOOST_AUTO_TEST_SUITE( TestSTLDataStructure )



// Given D = { d1, d2, .. dN }, find all {i,j} pairs 
//    where | xj - xi | - N * pitchx <= epi && | yj - yi | <= epi. 
//    N can be any integer

struct Defect {
	int x, y;
};

typedef vector< pair<int,int> >  PairVectors;


void find_pitched_pairs_with_loops( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs )
{	
	vector<bool> isLP( N, 0 );

	for( size_t i=0; i<N; ++i ) {
		if( isLP[i] ) continue;

		for( size_t j =0; j<N; ++j ) {
			if( i == j ) continue;
			if( isLP[j] ) continue;

			int dx = abs( D[i].x - D[j].x );
			int dy = abs( D[i].y - D[j].y );
			int dxr = (int)abs( dx - pitch* (int)( dx/pitch + 0.5) ); 

			if( dxr <= epi && dy <= epi ) {
				pairs.push_back( make_pair<int,int>( (int)i, (int)j ) );
				isLP[i] = isLP[j] = true;
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( UsingLoops ) 
{  
	Defect D[] = { { 10, 10 }, { 100, 12 },  { 110, 10 }, { 200, 12 }, {410, 10 } };
	size_t N = 5;

	PairVectors pairs;
	find_pitched_pairs_with_loops( D, N, 100.0, 1, pairs );

	BOOST_CHECK_EQUAL( pairs.size(), 3 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 0 && pairs[1].second == 4 );
	BOOST_CHECK( pairs[2].first == 1 && pairs[2].second == 3 );


	pairs.clear();
	find_pitched_pairs_with_loops( D, N, 100.5, 1, pairs );

	BOOST_CHECK_EQUAL( pairs.size(), 2 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 1 && pairs[1].second == 3 );

}


void find_pitched_pairs_with_two_dimensional_array( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs, int W, int H )
{	
	if( epi > 1 )  throw Exception( "not supported" );
	
	vector<uint16_t> M( W * H, 0xFFFF );
	vector<bool> isLP( N, false );

	for( size_t i=0; i<N; ++i ) { 
		M[ D[i].x + D[i].y * W ] = i; 
	}

	for( size_t i=0; i<N; ++i ) {
		if( isLP[i] ) continue;

		const Defect& d( D[i] );

		double px = d.x + pitch;
		for( ; px < W; px += pitch ) {
			uint16_t* m = &(M[ (int)(px + d.y*W) ]);
			if( *m != 0xFFFF || *(++m) != 0xFFFF )
			{
				isLP[i]=true;
				isLP[ *m ]=true;
				pairs.push_back( make_pair<int,int>( (int)i, (int)(*m)) );
			}
			else
			{
				break;
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( UsingArray ) 
{  
	Defect D[] = { { 10, 10 }, { 100, 12 },  { 110, 10 }, { 200, 12 }, {410, 10 } };
	size_t N = 5;

	PairVectors pairs;
	find_pitched_pairs_with_two_dimensional_array( D, N, 100.0, 1, pairs, 640, 480 );

	BOOST_CHECK_EQUAL( pairs.size(), 2 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 1 && pairs[1].second == 3 );
	// Note that 410,10 is not caught
	
	pairs.clear();
	find_pitched_pairs_with_two_dimensional_array( D, N, 100.5, 1, pairs, 640, 480 );

	BOOST_CHECK_EQUAL( pairs.size(), 2 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 1 && pairs[1].second == 3 );
}



struct DefectTag
{
	const Defect* Defect;
	int Index;
	bool IsLP;

	DefectTag( const struct Defect* d, int n ) : Defect(d), Index(n), IsLP(false) {};
};
typedef boost::shared_ptr<DefectTag> DefectTagPtr;


struct OrderDefectTagX {
	bool operator()(const DefectTagPtr& left, const DefectTagPtr& right) const {
		return left->Defect->x < right->Defect->x;
	}
};


typedef set<DefectTagPtr,OrderDefectTagX> DefectTagSet;
typedef vector< DefectTagSet > DefectTagSetVector;

void find_pitched_pairs_with_set_array( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs, int width, int height )
{	
	int& H = height;
	DefectTagSetVector SV( H );

	for( size_t i=0; i<N; ++i ) { 
		SV[ D[i].y ].insert( DefectTagPtr( new DefectTag( &D[i], i ) ));
	}

	Defect d = { 0, 0 };
	DefectTagPtr dt( new DefectTag( &d, 0 ) );

	for( size_t i=0; i<N; ++i ) { 
		DefectTagSet& S( SV[ D[i].y ] );
		
		d.x = D[i].x;
		DefectTagPtr base( *(S.find( dt )) );
		if( base->IsLP ) continue;

		double px = D[i].x + pitch;
		for( ; ; px += pitch) {
			d.x = (int)(px - epi);
			DefectTagSet::iterator low = S.lower_bound( dt );

			d.x = (int)(px + epi);
			DefectTagSet::iterator up = S.upper_bound( dt );

			for( ; low != up ; low++ ) {
				const Defect* t = (*low)->Defect;
				if( px-epi <= t->x && t->x <= px+epi ) {
					break;
				}
			}

			if( low != up ) {
				base->IsLP = true;
				(*low)->IsLP = true;
				pairs.push_back( make_pair<int,int>( base->Index, (*low)->Index));
			} else {
				break;
			}
		}
	}
}


BOOST_AUTO_TEST_CASE( UsingSetVector ) 
{  
	Defect D[] = { { 10, 10 }, { 100, 12 },  { 110, 10 }, { 200, 12 }, {410, 10 } };
	size_t N = 5;

	PairVectors pairs;
	find_pitched_pairs_with_set_array( D, N, 100.0, 1, pairs, 640, 480 );
	BOOST_CHECK_EQUAL( pairs.size(), 2 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 1 && pairs[1].second == 3 );
	// Note that 410,10 is not caught

	pairs.clear();
	find_pitched_pairs_with_set_array( D, N, 100.5, 1, pairs, 640, 480 );

	BOOST_CHECK_EQUAL( pairs.size(), 2 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 1 && pairs[1].second == 3 );
}



BOOST_AUTO_TEST_CASE( TestManyFalseDefectTiming ) 
{  
	const int N = 1024;
	Defect D[N];
	for( int i=0; i<N; ++i ){
		D[i].x = i%N * 10;
		D[i].y = i%N;
	}
	
	D[100].x = 105;
	D[100].y = 200;
	D[200].x = 207;
	D[200].y = 200;

	PairVectors pairs;
	find_pitched_pairs_with_loops( D, N, 102.0, 1, pairs );
	BOOST_CHECK_EQUAL( pairs.size(), 1 );
	BOOST_CHECK( pairs[0].first == 100 && pairs[0].second == 200 );


	pairs.clear();
	find_pitched_pairs_with_two_dimensional_array( D, N, 102.0, 1, pairs, 1024*12, 1024 );
	BOOST_CHECK_EQUAL( pairs.size(), 1 );
	BOOST_CHECK( pairs[0].first == 100 && pairs[0].second == 200 );

	
	pairs.clear();
	find_pitched_pairs_with_set_array( D, N, 102.0, 1, pairs, 1024*12, 1024 );
	BOOST_CHECK_EQUAL( pairs.size(), 1 );
	BOOST_CHECK( pairs[0].first == 100 && pairs[0].second == 200 );


	const int LOOP = 10;
	StopWatch w;
	double elapsed = 0.0;

	printf( _T("\nRunning %d false defects with %d loop"), N, LOOP );

	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		find_pitched_pairs_with_loops( D, N, 102.0, 1, pairs );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_loops takes %.3f second"), elapsed / LOOP );
	
	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		find_pitched_pairs_with_two_dimensional_array( D, N, 102.0, 1, pairs, 1024*12, 1024 );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_two_dimensional_array takes %.3f seconds"), elapsed / LOOP );

	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		find_pitched_pairs_with_set_array( D, N, 102.0, 1, pairs, 1024*12, 1024 );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_set_array takes %.3f seconds"), elapsed / LOOP );
}



BOOST_AUTO_TEST_CASE( TestManyTrueDefectTiming ) 
{  
	const int N = 1024;
	Defect D[N];
	int x = 0, y=0;
	for( int i=0; i<N; ++i ){
		D[i].x = x;
		D[i].y = y;

		x += 100;
		if( x > 1024*12 ) {
			x = 0;
			y += 20;
		}
	}

	const int LOOP = 10;
	StopWatch w;
	double elapsed = 0.0;
	PairVectors pairs;

	printf( _T("\nRunning %d true defects with %d loop"), N, LOOP );

	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		pairs.reserve(1024);
		find_pitched_pairs_with_loops( D, N, 100.0, 1, pairs );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_loops takes %.3f second"), elapsed / LOOP );
	
	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		pairs.reserve(1024);
		find_pitched_pairs_with_two_dimensional_array( D, N, 100.0, 1, pairs, 1024*12, 1024 );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_two_dimensional_array takes %.3f seconds"), elapsed / LOOP );

	w.Reset();
	for( int i=0; i<LOOP; ++i) {
		pairs.clear();
		pairs.reserve(1024);
		find_pitched_pairs_with_set_array( D, N, 100.0, 1, pairs, 1024*12, 1024 );
	}
	elapsed = w.Elapsed();
	printf( _T("\nfind_pitched_pairs_with_set_array takes %.3f seconds"), elapsed / LOOP );
}



BOOST_AUTO_TEST_SUITE_END() 