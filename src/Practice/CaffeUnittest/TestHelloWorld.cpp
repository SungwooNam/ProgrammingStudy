#include "stdafx.h"

#include "Fixture.hpp"

// Tried with VS2013 C++, but protobuf 2.5 does not work with Caffe. 
// #include "caffe\caffe.hpp"

using namespace std;

class TestHelloWorld : public CppUnit::TestCase
{
public:
	TestHelloWorld(const string& name) : CppUnit::TestCase(name) {}
	void setUp() {}
	void tearDown() {}

	void Test()
	{
		
	}

	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestHelloWorld");
		
		CppUnit_addTest(pSuite, TestHelloWorld, Test);

		return pSuite;
	}
};

AddTestCase<TestHelloWorld> s_Test;
