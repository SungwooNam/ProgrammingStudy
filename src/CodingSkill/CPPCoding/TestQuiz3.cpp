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

struct StudentScore
{
	int ID;
	int Score;
	int Date;
};
typedef list<int> Scores;

class ScoreBoard
{
public:
	typedef map<int, Scores> Board;

	struct InDescendingOrder
	{
		bool operator()( int x, int y )
		{
			return x > y;
		}
	};

	void Process( StudentScore* scores, int scoreCount )
	{
		// make a map with id and scores as list
		Board board;
		for( int i=0; i<scoreCount; ++i )
		{
			const StudentScore& s( scores[i] );
			board[ s.ID ].push_back( s.Score );
		}

		// iterate each item in the map and sort and take first 5 and make average
		Board::iterator B;
		for( B = board.begin(); B!= board.end(); ++B )
		{
			Scores& scores(B->second);
			scores.sort( InDescendingOrder() );

			int count = 0; 
			int sum = 0;
			for( Scores::iterator S=scores.begin(); S!=scores.end() && count<5; ++S, ++count) 
			{
				sum += *S;
			}

			double average = (double)sum / count;
		}
	}
};


BOOST_AUTO_TEST_CASE( TestQuiz3 )
{
	StudentScore scores[] = { 
		{ 0, 89, 112233 }, 
		{ 0, 90, 112233 }, 
		{ 0, 91, 112233 }, 
		{ 0, 89, 112233 }, 
		{ 0, 99, 112233 }, 
		{ 0, 89, 112233 } }; 

	ScoreBoard board;
	board.Process( scores, 6 );

}