#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <boost\shared_ptr.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestSmartPointer )

BOOST_AUTO_TEST_CASE( SharedPointerBasic ) 
{  
	boost::shared_ptr< char > name( new char[100] );
	strcpy_s( name.get(), 100, "SharedPointerBasic" );
	BOOST_CHECK_EQUAL( name.get(), "SharedPointerBasic" );

	boost::shared_ptr< char > another_name = name;
	BOOST_CHECK_EQUAL( another_name.get(), "SharedPointerBasic" );

	name.reset();
	strcpy_s( another_name.get(), 100, "Changed!!" );
}

typedef boost::shared_ptr< char > CharPtr;

void UpdateName( CharPtr n, size_t nsize )
{
	strcpy_s( n.get(), nsize, "New name" );
}


BOOST_AUTO_TEST_CASE( SharedPointerNoMemoryLeakWithException ) 
{  
	try
	{
		boost::shared_ptr< char > name( new char[100] );
		strcpy_s( name.get(), 100, "name In SP" );

		UpdateName( name, 100 );
		
		throw "bang!";
	}
	catch(...)
	{
	}
}




class Servo
{
public:
	Servo() : m_Position( 0.0 ) {}
	~Servo() { m_Position = 1234.0; }

	double GetPosition() const { return m_Position; }
	void SetPosition( double pos ) { m_Position = pos; }

private:
	double m_Position;
};

typedef boost::shared_ptr<Servo> ServoPtr;

BOOST_AUTO_TEST_CASE( SharedPointerWithClass ) 
{  
	ServoPtr x( new Servo() );
	x->SetPosition( 1.0 );

	BOOST_CHECK_EQUAL( x->GetPosition(), 1.0 );

	ServoPtr monitor_x = x;
	BOOST_CHECK_EQUAL( monitor_x->GetPosition(), 1.0 );

	x->SetPosition( 2.0 );
	BOOST_CHECK_EQUAL( monitor_x->GetPosition(), 2.0 );
}

BOOST_AUTO_TEST_CASE( SharedScopeRule ) 
{  
	{
		ServoPtr monitor_x;

		{
			ServoPtr x( new Servo() );
			x->SetPosition( 1.0 );
			monitor_x = x;
		}

		BOOST_CHECK_EQUAL( monitor_x->GetPosition(), 1.0 );
	}
}


BOOST_AUTO_TEST_SUITE_END() 