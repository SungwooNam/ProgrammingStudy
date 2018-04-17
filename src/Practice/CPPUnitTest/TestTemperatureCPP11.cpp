#include "stdafx.h"

#include "Fixture.hpp"
#include <string>
#include <iostream>
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
		static const char *symbol() { return "C"; }
		static value_type water_freezing_threshold() { return 0; }
		static value_type to_neutral(value_type val) { return val; }
		static value_type from_neutral(value_type val) { return val; }
	};

	template <> struct temperature_traits<Fahrenheit> : base_temperature_traits{
		static const char *symbol() { return "F"; }
		static value_type water_freezing_threshold() { return 32; }
		static value_type to_neutral(value_type val) { return (val - water_freezing_threshold()) * 5 / 9; }
		static value_type from_neutral(value_type val) { return val * 9 / 5 + water_freezing_threshold(); }
	};

	template <> struct temperature_traits<Kelvin> : base_temperature_traits{
		static const char *symbol() { return "K"; }
		static value_type water_freezing_threshold() { return 273.15; }
		static value_type to_neutral(value_type val) { return val - water_freezing_threshold(); }
		static value_type from_neutral(value_type val) { return val + water_freezing_threshold(); }
	};

	template <class R>
	using temp_val_t = typename temperature_traits<R>::value_type;

	class exact {};
	class floating_pt {};

	template <class T>
	T absolute(T arg) {
		return arg < 0 ? -arg : arg;
	}

	template <class T>
	bool close_enough(T a, T b, floating_pt) {
		return absolute(a - b) < static_cast<T>(0.000001);
	}
	template <class T>
	bool close_enough(T a, T b, exact) {
		return a == b;
	}
	template <class T>
	bool close_enough(T a, T b) {
		return close_enough(a, b, typename conditional<
			is_floating_point<T>::value, floating_pt, exact
		>::type{});
	}

	template <class D, class S>
	temp_val_t<D> temperature_cast(const temp_val_t<D> val) {
		return temperature_traits<D>::from_neutral(
			temperature_traits<S>::to_neutral(val)
			);
	}

	template <class R>
	class Temperature {
	public:
		using value_type = temp_val_t<R>;
	private:
		value_type val = temperature_traits<R>::water_freezing_threshold();
	public:
		Temperature() = default;
		Temperature(value_type val) : val{ val } {
		}
		template <class T>
		Temperature(const Temperature<T> &other) : val{ temperature_cast<R, T>(other.value()) } {
		}
		template <class T>
		Temperature& operator=(const Temperature<T> &other) {
			val = temperature_cast<R, T>(other.value());
			return *this;
		}
		value_type value() const {
			return val;
		}
		bool operator==(const Temperature &other) const {
			return close_enough(value(), other.value());
		}
		bool operator!=(const Temperature &other) const {
			return !(*this == other);
		}
		bool operator<(const Temperature &other) const {
			return value() < other.value();
		}
		bool operator<=(const Temperature &other) const {
			return value() <= other.value();
		}
		bool operator>(const Temperature &other) const {
			return value() > other.value();
		}
		bool operator>=(const Temperature &other) const {
			return value() >= other.value();
		}
		static const char * symbol() {
			return temperature_traits<R>::symbol();
		}
		Temperature operator-() const {
			return{ -value() };
		}
		friend ostream& operator<<(ostream &os, const Temperature &temp) {
			return os << temp.value() << temp.symbol();
		}
	};
}

class TestTemperatureCPP11 : public CppUnit::TestCase
{
public:
	TestTemperatureCPP11(const string& name) : CppUnit::TestCase(name) {}

	void test()
	{
		using namespace CPP11_Temperature;
	}

	static CppUnit::Test* CreateTest()
	{
		CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TestTemperatureCPP98");

		CppUnit_addTest(pSuite, TestTemperatureCPP11, test);

		return pSuite;
	}
};

AddTestCase<TestTemperatureCPP11> s_Test;