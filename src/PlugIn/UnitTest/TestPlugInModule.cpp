#include <stdexcept>
#include <boost\test\unit_test.hpp>

#include "PlugIn\PlugInModule.hpp"
#include "IFooBar.h"

using namespace std;
using namespace ProgrammingStudy;
using namespace ProgrammingStudy::PlugIn;


BOOST_AUTO_TEST_SUITE( TestComponents )

BOOST_AUTO_TEST_CASE( TestBasic )
{  
	PlugInModulePtr plugInFoo( new PlugInModule() );

	plugInFoo->Load( _T("PlugInFoo.dll") );
	const DLLVERSIONINFO& version( plugInFoo->GetVersionInfo() );
	BOOST_CHECK( version.dwMajorVersion == 1 && version.dwMinorVersion == 0 && version.dwBuildNumber == 1 );

	IFooPtr foo( plugInFoo->CreatePlugIn<IFoo>( "SSKL.PlugInFoo.1" ) );
	foo->GetID();
		
	BOOST_CHECK( string("SSKL.PlugInFoo.1") == foo->GetID() );
	BOOST_CHECK( 100 == foo->Foo( 10 ) );

	IBarPtr bar( plugInFoo->CreatePlugIn<IBar>( "SSKL.PlugInBar.1" ) );
	bar->GetID();
		
	BOOST_CHECK( string("SSKL.PlugInBar.1") == bar->GetID() );
	BOOST_CHECK( 40 == bar->Bar( 2, 20 ) );


	Exception caughtException(_T(""));
	try 
	{
		bar->Bar( 0, 1 );
	}
	catch( Exception& ex )
	{
		caughtException = ex;
	}

	BOOST_CHECK( caughtException.Message() != _T("") );
}

BOOST_AUTO_TEST_CASE( TestInterfaceDiscovery )
{  
	// FIX ME : Multiple inheritance using interface class ( pure abstract class ) does not work
	// Below bars.size() should be 2 and loos.size() should be 1. 

	PlugInModulePtr module( new PlugInModule() );
	module->Load( _T("PlugInFoo.dll") );

	PlugInNameArray ids( module->GetPlugInIDs() );

	{
		std::list< IBarPtr > bars;
		for( PlugInNameArray::iterator I=ids.begin(); I!=ids.end(); ++I )
		{
			const std::string& id( *I );

			try 
			{
				IBarPtr bar( module->CreatePlugIn<IBar>( id.c_str() ) );
				bars.push_back( bar );
			
			}
			catch( Exception )
			{
			}
		}

		BOOST_CHECK( bars.size() == 1 );

		bars.clear();
	}
	
	{
		std::list< ILooPtr > loos;
		for( PlugInNameArray::iterator I=ids.begin(); I!=ids.end(); ++I )
		{
			const std::string& id( *I );

			try 
			{
				ILooPtr loo( module->CreatePlugIn<ILoo>( id.c_str() ) );
				loos.push_back( loo );
			
			}
			catch( Exception )
			{
			}
		}

		BOOST_CHECK( loos.size() == 0 );

		loos.clear();
	}

}



BOOST_AUTO_TEST_CASE( TestPlugInModuleManager )
{  
	PlugInModuleManagerPtr manager( new PlugInModuleManager() );

	manager->Load( _T("PlugInFoo.dll") );
	IFooPtr foo( manager->CreatePlugIn<IFoo>( "SSKL.PlugInFoo.1" ) );
		
	BOOST_CHECK( string("SSKL.PlugInFoo.1") == foo->GetID() );
	BOOST_CHECK( 100 == foo->Foo( 10 ) );

	IBarPtr bar( manager->CreatePlugIn<IBar>( "SSKL.PlugInBar.1" ) );
		
	BOOST_CHECK( string("SSKL.PlugInBar.1") == bar->GetID() );
	BOOST_CHECK( 40 == bar->Bar( 2, 20 ) );


	Exception caughtException(_T(""));
	try 
	{
		manager->CreatePlugIn<IBar>( "SSKL.PlugInBar.2" );
	}
	catch( Exception& ex )
	{
		caughtException = ex;
	}

	BOOST_CHECK( caughtException.Message() != _T("") );
}

BOOST_AUTO_TEST_SUITE_END()