#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <boost\shared_ptr.hpp>
#include <string>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestDevelopment )

class IGlass
{
public:
	virtual ~IGlass() {}
	virtual string GetName() const = 0;
};

typedef boost::shared_ptr< IGlass > IGlassPtr;


class IEquipment
{
public:
	virtual ~IEquipment() {}
	virtual void Place( IGlassPtr glass ) = 0;
	virtual IGlassPtr Remove() = 0; 
};

typedef boost::shared_ptr< IEquipment > IEquipmentPtr;



class Glass : public IGlass
{
public:
	Glass( const char * name ) { _Name = name; }
	string GetName() const { return _Name; }

private:
	string _Name;
};

class Chamber : public IEquipment
{
public:
	void Place( IGlassPtr glass ) 
	{
		if( _Glass != NULL )
		{
			throw std::exception( "There is already glass on this equipment" );
		}

		_Glass = glass;
	}

	IGlassPtr Remove() 
	{
		IGlassPtr removed = _Glass;
		_Glass = NULL;
		return removed;
	}

private:
	IGlassPtr _Glass;

};



BOOST_AUTO_TEST_CASE( ShouldPlaceRemoveOneByOne )
{  
	IEquipmentPtr chamber( new Chamber() );
	IGlassPtr glass( new Glass("TestGlass") );

	chamber->Place( glass );
	IGlassPtr removed = chamber->Remove();
	BOOST_CHECK_EQUAL( removed->GetName(), "TestGlass" );	
}


BOOST_AUTO_TEST_SUITE_END()