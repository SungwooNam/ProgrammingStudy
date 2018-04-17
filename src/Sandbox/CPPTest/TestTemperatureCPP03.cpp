#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>

using namespace std;

namespace CPP03_Temperature
{
	class Celsius;
	class Fahrenheit;
	class Kelvin;

	struct base_temperature_traits {
		typedef double value_type;
	};

	template <class> struct temperature_traits;
	template <> struct temperature_traits<Celsius> : base_temperature_traits{
		static const char *symbol() { return "C"; }
		static value_type water_freezing_threshold() { return 0; }
		static value_type to_neutral(value_type val) { return val;  }
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

	template <bool, class IfTrue, class IfFalse> struct static_if_else;
	template <class IfTrue, class IfFalse> struct static_if_else<true, IfTrue, IfFalse> {
		typedef IfTrue type;
	};
	template <class IfTrue, class IfFalse> struct static_if_else<false, IfTrue, IfFalse> {
		typedef IfFalse type;
	};

	class exact {};
	class floating_pt {};
	
	template <class> struct is_floating { enum { value = false }; };
	template <> struct is_floating<float> { enum { value = true }; };
	template <> struct is_floating<double> { enum { value = true }; };
	template <> struct is_floating<long double> { enum { value = true }; };

	template <class T> bool close_enough(T a, T b, floating_pt) {
		return abs(a - b) < static_cast<T>(0.000001);
	}

	template <class T> bool close_enough(T a, T b, exact) {
		return a == b;
	}

	template <class T> bool close_enough(T a, T b) {
		return close_enough( a, b, 
			typename static_if_else< is_floating<T>::value, floating_pt, exact >::type() );
	}

	template < class D, class S >
	typename temperature_traits<D>::value_type 
		temperature_cast(const typename temperature_traits<D>::value_type val) {
			return temperature_traits<D>::from_neutral( 
				temperature_traits<S>::to_neutral(val) );
		}

	template <class R>
	class Temperature {
	public:
		typedef typename temperature_traits<R>::value_type value_type;
	private:
		value_type val;
	public:
		Temperature(value_type val = temperature_traits<R>::water_freezing_threshold()) :
			val(val) {}

		template <class T>
		Temperature(const Temperature<T> &other) : val(temperature_cast<R, T>(other.value())) {}

		template< class T>
		Temperature& operator=(const Temperature<T> &other) {
			val = temperature_cast<R, T>(other.value());
			return *this;
		}
		value_type value() const { return val; }
		bool operator==(const Temperature &other) const { return close_enough(value(), other.value()); }
		bool operator!=(const Temperature &other) const { return !(*this == other); }
		bool operator<(const Temperature &other) const { return value() < other.value(); }
		bool operator<=(const Temperature &other) const { return value() <= other.value(); }
		bool operator>(const Temperature &other) const { return value() > other.value(); }
		bool operator>=(const Temperature &other) const { return value() >= other.value(); }
		static const char * symbol() { return temperature_traits<R>::symbol(); }
		friend ostream& operator<<(ostream& os, const Temperature& temp) {
			return os << temp.value() << temp.symbol();
		}
	};
}

BOOST_AUTO_TEST_SUITE( TestTemperatureCPP03 )

BOOST_AUTO_TEST_CASE(TestBasic)
{
	using namespace CPP03_Temperature;

	Temperature<Fahrenheit> fahr = 41;
	Temperature<Celsius> cels = fahr;
	BOOST_CHECK_EQUAL(close_enough(cels.value(), 5.0), true);
	BOOST_CHECK_EQUAL(close_enough(cels, Temperature < Celsius>(5)), true);
	Temperature<Kelvin> kelv = 0;
	cels = kelv;
	BOOST_CHECK_EQUAL(close_enough(cels.value(), -273.15),true);
	BOOST_CHECK_EQUAL(close_enough(cels, Temperature<Celsius>(-273.15)), true);
	cels = 0;
	BOOST_CHECK_EQUAL( 
		close_enough(Temperature<Fahrenheit>(cels), Temperature<Fahrenheit>(32)),
		true);
	cout << cels << " == " << Temperature<Fahrenheit>(cels) << endl;
}

BOOST_AUTO_TEST_SUITE_END()