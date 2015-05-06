#include "StdAfx.h"

#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include <math.h>
#include "Exception.hpp"

using namespace std;

BOOST_AUTO_TEST_SUITE( TestSTLDataStructure )



// Given D = { d1, d2, .. dN }, find all {i,j} pairs 
//    where | xj - xi | - N * pitchx <= epi && | yj - yi | <= epi. 
//    N can be any integer

struct Defect
{
	int x, y;
};

typedef vector< pair<int,int> >  PairVectors;




void find_pitched_pairs_with_loops( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs )
{	
	vector<int> s( N, 0 );

	for( size_t i=0; i<N; ++i ) {
		for( size_t j =0; j<N; ++j ) {
			if( i == j ) continue;
			if( s[j] != 0 ) continue;

			int dx = abs( D[i].x - D[j].x );
			int dy = abs( D[i].y - D[j].y );
			int dxr = (int)abs( dx - pitch* (int)( dx/pitch + 0.5) ); 

			if( dxr <= epi && dy <= epi ) {
				pairs.push_back( make_pair<int,int>( i, j ) );
				s[i] = s[j] = 1;
			}
		}
	}
}

BOOST_AUTO_TEST_CASE( UsingLoops ) 
{  
	Defect D[] = { { 10, 10 }, { 100, 12 },  { 110, 10 }, { 100, 12 }, {310, 10 } };

	PairVectors pairs;
	find_pitched_pairs_with_loops( D, 5, 100.0, 0, pairs );

	BOOST_CHECK_EQUAL( pairs.size(), 3 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[1].first == 0 && pairs[1].second == 4 );
	BOOST_CHECK( pairs[2].first == 1 && pairs[2].second == 3 );


	pairs.clear();
	find_pitched_pairs_with_loops( D, 5, 100.5, 0, pairs );

	BOOST_CHECK_EQUAL( pairs.size(), 3 );
	BOOST_CHECK( pairs[0].first == 0 && pairs[0].second == 2 );
	BOOST_CHECK( pairs[2].first == 1 && pairs[2].second == 3 );

}



void find_pitched_pairs_with_sets( const Defect* D, size_t N, double pitch, int epi, PairVectors& pairs )
{	
	if( epi > 1 ) throw ProgrammingStudy::Exception( "not supported" );


}



BOOST_AUTO_TEST_SUITE_END() 