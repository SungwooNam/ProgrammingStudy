#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestConstness )

BOOST_AUTO_TEST_CASE( TestConstReference ) 
{  
	int a = 1234;
	const int &b = a;

	BOOST_CHECK_EQUAL( b, 1234 );
	BOOST_CHECK_EQUAL( &b, &a );

#if 0
	b = 3;		// of course compile error
#endif

	int &c = a;
	c = 4321;
	BOOST_CHECK_EQUAL( a, 4321 );
}

class Foo
{
public:
	Foo() : m_Counter(0) {}

	void Increase() { m_Counter++; }
	int Count() const { return m_Counter; }

private:
	int m_Counter;
};


BOOST_AUTO_TEST_CASE( TestConstFunctionsAndReturn ) 
{  
	Foo f1;
	f1.Increase();
	BOOST_CHECK_EQUAL( f1.Count(), 1 );
}



class FooFactory
{
public:
	const Foo& GetConservative() const { return m_Conservative; }
	Foo& GetLiberal() { return m_Liberal; }

private:
	Foo m_Conservative;
	Foo m_Liberal;
};

BOOST_AUTO_TEST_CASE( TestConstReturn ) 
{  
	FooFactory factory;
	const Foo& chanhi = factory.GetConservative();
#if 0
	chanhi.Increase();	// Compile error : Remember that dull is const !
#endif 
	BOOST_CHECK_EQUAL( chanhi.Count(), 0 );

	Foo& sungwoo( factory.GetLiberal() );
	sungwoo.Increase();	
	BOOST_CHECK_EQUAL( sungwoo.Count(), 1 );

	Foo& fakeSungwoo( factory.GetLiberal() );
	fakeSungwoo.Increase();	
	BOOST_CHECK_EQUAL( sungwoo.Count(), 2 );
}



BOOST_AUTO_TEST_SUITE_END()