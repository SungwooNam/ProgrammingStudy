#include "stdafx.h"

#include "Fixture.hpp"
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;

// Substitution Failure Is Not Error

template <typename ... Ts > using void_t = void;
template <typename T, typename = void> struct has_typedef_foobar : std::false_type {};
template <typename T> 
struct has_typedef_foobar <
	T,
	void_t < typename T::foobar >>
	: std::true_type{};

struct foo {
	using foobar = float;
};


class TestSFINE : public CppUnit::TestCase
{
public:
	TestSFINE(const string& name) : CppUnit::TestCase(name) {}

	void test()
	{
		cout << std::boolalpha;
		cout << has_typedef_foobar<int>::value << endl;
		cout << has_typedef_foobar<foo>::value << endl;
	}

	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestSFINE");

		CppUnit_addTest(pSuite, TestSFINE, test);

		return pSuite;
	}
};

AddTestCase<TestSFINE> s_Test;