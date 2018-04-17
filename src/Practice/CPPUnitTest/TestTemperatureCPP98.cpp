#include "stdafx.h"

#include "Fixture.hpp"
#include <string>
#include <iostream>

using namespace std;

namespace CPP98_Temperature
{
	typedef double temperature_t;
	const char *SYMBOL_CELSIUS = "C",
		*SYMBOL_FAHRENHEIT = "F",
		*SYMBOL_KELVIN = "K";

	bool close_enough(temperature_t a, temperature_t b)
	{
		return abs(a - b) < 0.000001;
	}

	temperature_t celsius_to_kelvin(temperature_t cels)
	{
		return cels + 273.15;
	}

	temperature_t kelvin_to_celsius(temperature_t kelv)
	{
		return kelv - 273.15;
	}

	temperature_t celsius_to_fahrenheit(temperature_t cels)
	{
		return cels * 9 / 5 + 32;
	}

	temperature_t fahrenheit_to_celsius(temperature_t fahr)
	{
		return (fahr - 32) * 5 / 9;
	}

	temperature_t fahrenheit_to_kelvin(temperature_t fahr) {
		return celsius_to_kelvin(fahrenheit_to_celsius(fahr));
	}

	temperature_t kelvin_to_fahrenheit(temperature_t kelv) {
		return celsius_to_fahrenheit(kelvin_to_celsius(kelv));
	}
}

class TestTemperatureCPP98 : public CppUnit::TestCase
{
public:
	TestTemperatureCPP98(const string& name) : CppUnit::TestCase(name) {}

	void test()
	{
		using namespace CPP98_Temperature;

		temperature_t fahr = 41;
		temperature_t cels = fahrenheit_to_celsius(fahr);
		assert(close_enough(cels, 5));
		temperature_t kelv = 0;
		cels = kelvin_to_celsius(kelv);
		assert(close_enough(cels, -273.15));
		cels = 0;
		assert(close_enough(celsius_to_fahrenheit(cels), 32));
		cout << cels << SYMBOL_CELSIUS << " == " << celsius_to_fahrenheit(cels) << SYMBOL_FAHRENHEIT << endl;
	}

	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestTemperatureCPP98");

		CppUnit_addTest(pSuite, TestTemperatureCPP98, test);

		return pSuite;
	}
};

AddTestCase<TestTemperatureCPP98> s_Test;
