#pragma once

#include "CppUnit/TestSuite.h"
#include "CppUnit/TestCase.h"
#include "CppUnit/TestCaller.h"
#include <functional>

struct Fixture
{
protected:
	CppUnit::TestSuite* m_TestSuite;

	Fixture()
	{
		m_TestSuite = new CppUnit::TestSuite("MainTestSuite");
	}

	~Fixture()
	{
	}

public:
	static Fixture& GetGlobal() 
	{ 
		static Fixture s_Fixture;
		return s_Fixture; 
	}

	void Init() 
	{
	}

	CppUnit::TestSuite* GetTestSuite() { return m_TestSuite; }

	CppUnit::TestSuite* DetachTestSuite() 
	{ 
		CppUnit::TestSuite* temp = m_TestSuite; 
		m_TestSuite = NULL; 
		return temp; 
	}

	void Exit()
	{
	}
};

/**
	Usage:

	class FooCase : public TestCase
	{
	...
		static CppUnit::Test* CreateTest()
		{
			CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("VersionInfoTest");
			CppUnit_addTest(pSuite, VersionInfoTest, testKernel32Version);
			return pSuite;
		}
	};

	AddTestCase<FooCase> s_FooCase;
*/
template<typename T>
class AddTestCase
{
public:
	AddTestCase()
	{
		CppUnit::TestSuite* suite = Fixture::GetGlobal().GetTestSuite();
		suite->addTest(T::CreateTest());
	}
};

template< typename TException >
std::string CatchExceptionMessage( std::function<void()> f)
{
	std::string caughtMsg;
	try { f(); }
	catch (TException& ex) { caughtMsg = ex.message(); }
	return caughtMsg;
}