#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <stdio.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestException )


const int EMERGENCY = 1;
const int LEFT_CLAMP_UP = 2;
const int LEFT_CLAMP_DOWN = 3;
const int LEFT_CLAMP_PUSH_UP = 3;


namespace IfOrDie
{

class IO 
{
public:
	bool Set( int port, bool value ) { return true; }	// return false means failure
	bool Get( int port ) const { return true; }			// but return in here means on/off
														// or how about "bool Get( int port, bool* value )" ? 
};

class Control
{
public:
	bool GlassClampOn() 
	{
		if( m_IO.Get( EMERGENCY ) == false )
			return false;

		if( m_IO.Set( LEFT_CLAMP_PUSH_UP, true ) == false || 
			m_IO.Get( LEFT_CLAMP_DOWN ) == true ) 
			return false;

		return true;;
	}

	bool MoveToAlignCamera() 
	{
		return false;;
	}

private:
	IO m_IO;
};

}

BOOST_AUTO_TEST_CASE( CheckReturnAlways )
{  
	IfOrDie::Control control;
	
	if( !control.GlassClampOn() ||
		!control.MoveToAlignCamera() )
	{
		printf( "Failed. If you want to know why, call Chanhy or SungJun or whoever. \n" );
	}
}


namespace NoMoreIf
{

class IO 
{
public:
	void Set( int port, bool value ) 
	{ 
		if( port > 4 ) 
			throw std::exception( "Port number should be less than 4" );
	}	
	bool Get( int port ) const 
	{ 
		if( port <0 || port > 3 ) 
			throw std::exception( "Port number should be in between [0:3]" );
		
		return true; 
	}
};

class Control
{
public:
	void GlassClampOn() 
	{
		if( m_IO.Get( EMERGENCY ) == false )
			throw std::exception( "Emergency! Call ChanHi! Call Sungjun!" );

		m_IO.Set( LEFT_CLAMP_PUSH_UP, true );

		if( m_IO.Get( LEFT_CLAMP_DOWN ) == true ) 
			throw std::exception( "Left clamp is not going up! Call HW engineer!" );
	}

	void MoveToAlignCamera() 
	{
		throw std::exception( "What camera ? Not equipped yet!" );
	}

private:
	IO m_IO;
};

}

BOOST_AUTO_TEST_CASE( ThrowExceptionIfSomethingGoesWrong )
{  
	NoMoreIf::Control control;
	
	try
	{
		control.GlassClampOn();
		control.MoveToAlignCamera();
	}
	catch( std::exception& ex )
	{
		printf( "Failed. %s\n", ex.what() );
	}
}

BOOST_AUTO_TEST_SUITE_END()