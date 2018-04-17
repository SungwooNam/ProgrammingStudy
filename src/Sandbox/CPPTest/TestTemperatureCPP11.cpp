#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <type_traits>

using namespace std;

namespace CPP11_Temperature
{
	class Celsius;
	class Fahrenheit;
	class Kelvin;

	struct base_temperature_traits { using value_type = double; };
	
	template <class> struct temperature_traits;
	template <> struct temperature_traits<Celsius> : base_temperature_traits{
		static constexpr const char *symbol() { return "C"; }
		static constexpr value_type water_freezing_threshold() { return 0; }
		static constexpr value_type to_neutral(value_type val) { return val; }
		static constexpr value_type from_neutral(value_type val) { return val; }
	};
}

BOOST_AUTO_TEST_SUITE( TestTemperatureCPP11 )

BOOST_AUTO_TEST_CASE(TestBasic)
{
	using namespace CPP1_Temperature;


}

BOOST_AUTO_TEST_SUITE_END()