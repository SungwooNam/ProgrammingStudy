#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <math.h>
#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <set>
#include <map>
#include "Exception.hpp"

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



struct OrderDefectX {
	bool operator()(const Defect* left, const Defect* right) const {
		return left->x < right->x;
	}
};

typedef set<const Defect*,OrderDefectX> DefectSet;
typedef boost::shared_ptr<DefectSet> DefectSetPtr;
typedef map<int,DefectSetPtr> DefectPlane;


void find_pitched_pairs_with_map_set( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs )
{	
	if( epi > 1 ) throw Exception( "not supported" );

	DefectPlane plane;
	for( size_t i=0; i<N; ++i ) { 
		DefectSetPtr ds;
		DefectPlane::iterator I = plane.find( D[i].y );
		if( I == plane.end() )
		{
			pair<DefectPlane::iterator,bool> result = 
				plane.insert( 
					make_pair<int,DefectSetPtr>( 
						D[i].y, 
						DefectSetPtr(new DefectSet())
					) 
				);
			I = result.first;
		}
		ds = I->second;
		ds->insert( &D[i] );
	}
}

BOOST_AUTO_TEST_CASE( UsingMapSet ) 
{  
	Defect D[] = { { 10, 10 }, { 100, 12 },  { 110, 10 }, { 200, 12 }, {410, 10 } };
	size_t N = 5;

	PairVectors pairs;
	find_pitched_pairs_with_map_set( D, N, 100.0, 1, pairs );
}




BOOST_AUTO_TEST_SUITE_END() 