/*
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


class Host
{
private:
	int m_HostID;
	int m_InstanceType;
	std::list<bool> m_Slots;

public:
	Host( int id, int type );
	void Add( int slot ) 
	{
		m_Slots.push_back( slot );
	}
};

typedef boost::shared_ptr<Host> HostPtr;

class HostLoader
{
public:
	std::list<HostPtr> LoadFromFile( const char *filename)
	{
		// FIX ME : need to use c++ stream class
		FILE *fp = fopen( filename, "rt" );
		if( fp == NULL )
			throw "Failed to open file";


		std::list<HostPtr> hosts;

		char line[255];
		while( fgets( line, 255, fp ) != NULL )
		{
			std::list<const char *> tokens;
			tokenizer( line, ',', tokens );

			std::list<const char *>::iterator I = tokens.begin();

			int hostid = atoi( *I ); ++I;
			int 
			

			std::vector<const char*> heads( tokens.begin(), tokens.begin()+2 );
			HostPtr host( new Host( atoi( heads[0]), atoi( heads[1] ) ) );
			std::list<const char *>::iterator I;
			for( I = tokens.begin()+3; I != tokens.end(); ++I )
			{
				const char *token = *I;
				bool isTaken = atoi( token ) == 1 ? true : false;
				host->Add( isTaken );
			}

			hosts.push_back( host );
		}

		fclose(fp);
	}

private:
	void tokenizer( char *line, char separator, std::list<const char*>& tokens )
	{
		char *c = line;
		bool isNewToken = true;

		while( *c != NULL )
		{
			if( isNewToken ) 
			{ 
				tokens.push_back( c );
				isNewToken = false;
			}

			if( *c == separator )
			{
				isNewToken = true;
				*c = NULL;
			}

			c++;
		}
	}
};



BOOST_AUTO_TEST_CASE( TestQuiz2Test1 )
{
	{
		FILE *fp = fopen( "temp.txt", "wt" );
		fprintf( fp, "1,M1,3,0,0,1\n");
		fprintf( fp, "2,M2,4,0,0,1,0\n");
		fprintf( fp, "3,M3,5,0,0,1,0,1\n");
		fclose(fp);
	}

	HostLoader loader;
	loader.LoadFromFile( "temp.txt" );
}

*/