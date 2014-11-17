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

// make hash table that 
// hash value from adding up characters
// comparison test - equal if all character can be found in other word

typedef set<string> Anagrams;
typedef boost::shared_ptr<Anagrams> AnagramsPtr;

class AnagramHash
{
public:
	size_t operator()( const string& w ) const
	{
		size_t r = 0;
		for( string::const_iterator I=w.begin(); I!=w.end(); ++I)
		{
			r += *I;
		}
		return r;
	}
};

class AnagramCompare
{
public:
	bool operator()( const string& x, const string& y ) const
	{
		if( x.length() != y.length() )
			return false;

		for( string::const_iterator I=x.begin(); I!=x.end(); ++I)
		{
			if( y.find( *I ) ==  string::npos )
				return false;
		}
		return true;
	}
};

typedef unordered_map< string, AnagramsPtr, AnagramHash, AnagramCompare > AnagramHashTable;

class AnagramFinder
{
public:
	void Find( string words[], int wordsCount )
	{
		AnagramHashTable table;
		for ( int i=0; i<wordsCount; ++i) 
		{
			AnagramHashTable::iterator I = table.find( words[i] );
			AnagramsPtr anagrams = NULL;

			if( I == table.end() )
			{
				anagrams = AnagramsPtr( new Anagrams() );
				table.emplace( pair<string,AnagramsPtr>( words[i], anagrams) );
			}
			else
			{
				anagrams = I->second;
			}

			anagrams->insert( words[i] );
		}


		AnagramHashTable::iterator A;
		for( A = table.begin(); A!=table.end(); ++A )
		{
			AnagramsPtr an = A->second;
			for( Anagrams::iterator W= an->begin(); W!=an->end();++W )
			{
				const string& word( *W );
				printf( "%s,", word.c_str() );
			}
			printf( "\n" );
		}
	}
};

BOOST_AUTO_TEST_CASE( TestQuiz4 )
{
	string words[] = { "CAT", "ACT", "TAC", "ACT", "GAC" };

	AnagramFinder finder;
	finder.Find( words, 5 );
}