#include "stdafx.h"

#include "Fixture.hpp"
#include <string>

using namespace std;

class TestFoo : public CppUnit::TestCase
{
public:
	TestFoo(const string& name) : CppUnit::TestCase(name) {}
	void setUp() {}
	void tearDown() {}

	void testFoo()
	{
	}

	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestFoo");
		
		CppUnit_addTest(pSuite, TestFoo, testFoo);

		return pSuite;
	}
};

AddTestCase<TestFoo> s_Test;
