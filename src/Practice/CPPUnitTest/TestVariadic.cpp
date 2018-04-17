#include "stdafx.h"

#include "Fixture.hpp"
#include <string>
#include <iostream>
#include <type_traits>

using namespace std;

void print(const char *s)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%')
			{
				++s;
			}
			else {
				throw std::runtime_error("invalid format string : missing arguments");
			}
		}

		std::cout << *s++;
	}
}

template<typename T, typename ...Args>
void print(const char *s, T value, Args ...args)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%')
			{
				++s;
			}
			else {
				std::cout << value;
				s += 2;
				print(s, args...);
				return;
			}
		}

		std::cout << *s++;
	}
}

template<typename T>
void foo(T arg)
{
	cout << arg;
}

template<typename ... Args> inline void pass(Args&&...) {}
template<typename ... Args> inline void expand(Args&&... args) {
	pass(foo(args)...);
}


class TestVariadic : public CppUnit::TestCase
{
public:
	TestVariadic(const string& name) : CppUnit::TestCase(name) {}

	void testPrint()
	{
		print("0:%d 1:%s 2:%f ", 1, "Hello", 3.14);
	}

	void testExpand()
	{
		// expand(42, "answer", true);
	}


	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestVariadic");

		CppUnit_addTest(pSuite, TestVariadic, testPrint);

		return pSuite;
	}
};

AddTestCase<TestVariadic> s_Test;