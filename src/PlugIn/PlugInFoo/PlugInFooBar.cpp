#include "stdafx.h"
#include "PlugIn\PlugIn.h"
#include "IFooBar.h"
#include "Exception.hpp"

HRESULT DllGetVersion( DLLVERSIONINFO *pvdi )
{
	pvdi->cbSize = sizeof( DLLVERSIONINFO );
	pvdi->dwPlatformID = DLLVER_PLATFORM_WINDOWS;
	pvdi->dwMajorVersion = 1;
	pvdi->dwMinorVersion = 0;
	pvdi->dwBuildNumber = 1;

	return S_OK;
}

namespace ProgrammingStudy {
namespace PlugIn {

class PlugInFoo : public IFoo
{
public:
	const char * GetID() const { return s_PlugInClassName; }
	int Foo( int i ) { return i * i; }

	static const char *GetClassName() { return s_PlugInClassName; }

private:
	static const char *s_PlugInClassName;
};

const char * PlugInFoo::s_PlugInClassName = "SSKL.PlugInFoo.1";


class PlugInBar : public IBar
{
public:
	const char * GetID() const { return s_PlugInClassName; }
	int Bar( int i, int j ) 
	{ 
		if( i == 0 )
		{
			throw Exception( StringFormat::As( _T("Not a good arguement %d"), i ) );
		}

		return i * j; 
	}
		
	static const char *GetClassName() { return s_PlugInClassName; }

private:
	static const char *s_PlugInClassName;
};

const char * PlugInBar::s_PlugInClassName = "SSKL.PlugInBar.1";



class PlugInFooBar : public IFoo, IBar
{
public:
	const char * GetID() const { return s_PlugInClassName; }
	int Foo( int i ) { return i * i; }
	int Bar( int i, int j ) 
	{ 
		if( i == 0 )
		{
			throw Exception( StringFormat::As( _T("Not a good arguement %d"), i ) );
		}

		return i * j; 
	}
		
	static const char *GetClassName() { return s_PlugInClassName; }

private:
	static const char *s_PlugInClassName;
};

const char * PlugInFooBar::s_PlugInClassName = "SSKL.PlugInFooBar.1";


class PlugInFooLoo : public IFoo, ILoo
{
public:
	const char * GetID() const { return s_PlugInClassName; }
	int Foo( int i ) { return i * i; }
	int Loo( int i, int j ) 
	{ 
		return i / j; 
	}
		
	static const char *GetClassName() { return s_PlugInClassName; }

private:
	static const char *s_PlugInClassName;
};

const char * PlugInFooLoo::s_PlugInClassName = "SSKL.PlugInFooLoo.1";


class FooPlugInFactory : public IPlugInFactory
{
public:
	const char * GetID() const { return "SSKL.PlugInFooFactory.1"; }

	PlugInNameArray GetPlugInIDs() const 
	{
		PlugInNameArray names;
		names.push_back( PlugInFoo::GetClassName() );
		names.push_back( PlugInBar::GetClassName() );
		names.push_back( PlugInFooBar::GetClassName() );
		names.push_back( PlugInFooLoo::GetClassName() );
		return names;
	}

	IPlugIn* Create( const char *plugInName ) 
	{
		std::string strPlugInName( plugInName );
		if( strPlugInName == PlugInFoo::GetClassName() )
		{
			return new PlugInFoo();
		}
		else if ( strPlugInName == PlugInBar::GetClassName() )
		{
			return new PlugInBar();
		}
		else if ( strPlugInName == PlugInFooBar::GetClassName() )
		{
			return (IFoo*)(new PlugInFooBar());
		}
		else if ( strPlugInName == PlugInFooLoo::GetClassName() )
		{
			return (new PlugInFooLoo());
		}
		else
			return NULL;
	}
};


}
}

ProgrammingStudy::PlugIn::IPlugInFactory* CPI_CreateFactory()
{
	return new ProgrammingStudy::PlugIn::FooPlugInFactory();
}
