#ifndef __PLUG_IN_LOADER_HPP__
#define __PLUG_IN_LOADER_HPP__

#include "PlugIn.h"
#include <boost/shared_ptr.hpp>
#include "Exception.hpp"
#include <map>
#include <algorithm>


namespace ProgrammingStudy {
namespace PlugIn {


class PlugInModule
{
public:
	PlugInModule() 
		: m_hDLL( NULL ) 
	{}

	~PlugInModule()
	{
		m_PlugInFactory.reset();

		if( m_hDLL ) {
			FreeLibrary( m_hDLL );
		}
	}

	void Load( const _TCHAR * filename )
	{
		m_hDLL = LoadLibrary( filename );
		if( m_hDLL == NULL ) 
		{
			throw Exception( StringFormat::As( _T("%s is not a valid DLL"), filename ) );
		}

		FN_DllGetVersion DllGetVersion = 
			(FN_DllGetVersion) GetProcAddress( m_hDLL, "DllGetVersion" );
		if( DllGetVersion == NULL )
		{
			throw Exception( StringFormat::As( _T("%s is not a supported PlugIn DLL"), filename ) );
		}

		if( S_OK != DllGetVersion( &m_VerInfo ) )
		{
			throw Exception( StringFormat::As( _T("Failed to get version info of the PlugIn DLL %s"), filename ) );
		}


		FN_CPI_CreateFactory CPI_CreateFactory = 
			(FN_CPI_CreateFactory) GetProcAddress( m_hDLL, "CPI_CreateFactory" );

		if( CPI_CreateFactory == NULL ) 
		{
			throw Exception( StringFormat::As( _T("%s is not a supported PlugIn DLL"), filename ) );
		}

		m_PlugInFactory = IPlugInFactoryPtr( CPI_CreateFactory() );
	}

	template<typename TPlugIn>
	boost::shared_ptr<TPlugIn> CreatePlugIn( const char *name )
	{
		if( m_PlugInFactory == NULL ) 
		{
			throw Exception( StringFormat::As( _T("No plugin in loaded") ) );
		}

		IPlugIn* plugIn = m_PlugInFactory->Create( name );
		if( plugIn == NULL ) 
		{
			throw Exception( StringFormat::As( _T("No plugin found with give name %s"), name ) );
		}

		TPlugIn* targetPlugIn = dynamic_cast< TPlugIn* > ( plugIn );
		if( targetPlugIn == NULL ) 
		{
			delete plugIn;

			throw Exception( StringFormat::As( _T("Can't convert to taregt plugin interface with %s"), name ) );
		}

		return boost::shared_ptr<TPlugIn>( targetPlugIn );
	}


	IPlugIn* UnsafeCreatePlugIn( const char *name )
	{
		if( m_PlugInFactory.get() == NULL ) 
		{
			throw Exception( StringFormat::As( _T("No plugin in loaded") ) );
		}

		IPlugIn* plugIn = m_PlugInFactory->Create( name );
		if( plugIn == NULL ) 
		{
			throw Exception( StringFormat::As( _T("No plugin found with give name %s"), name ) );
		}

		return plugIn;
	}

	const DLLVERSIONINFO& GetVersionInfo() const 
	{ 
		if( m_hDLL == NULL ) 
		{
			throw Exception( StringFormat::As( _T("No DLL has been loaded yet" ) ) );
		}

		return m_VerInfo; 
	}

	PlugInNameArray GetPlugInIDs() const
	{
		return m_PlugInFactory->GetPlugInIDs();
	}
	
private:
	typedef boost::shared_ptr<IPlugInFactory> IPlugInFactoryPtr;
	typedef IPlugInFactory* (* FN_CPI_CreateFactory)();
	typedef HRESULT (* FN_DllGetVersion)(DLLVERSIONINFO *);

	HMODULE m_hDLL;
	IPlugInFactoryPtr m_PlugInFactory;
	DLLVERSIONINFO m_VerInfo;
};


typedef boost::shared_ptr<PlugInModule> PlugInModulePtr;


class PlugInModuleManager
{
public:
	PlugInModuleManager() {}
	~PlugInModuleManager() 
	{
		UnloadAll();
	}

	void Load( const _TCHAR * filename )
	{
		ModuleContainer::iterator I = m_LoadedModules.find( _tstring(filename) );
		if( I != m_LoadedModules.end() ) 
		{
			throw Exception( StringFormat::As( _T("%s is already loaded. Unload first"), filename ) );
		}

		PlugInModulePtr module( new PlugInModule() );
		module->Load( filename );
		m_LoadedModules[ _tstring(filename) ] = module;

		PlugInNameArray ids( module->GetPlugInIDs() );
		PlugInNameArray::const_iterator NI;
		for( NI = ids.begin(); NI != ids.end(); ++NI )
		{
			const std::string& id( *NI );
			IDContainer::iterator II = m_IDModules.find( id );
			if (II != m_IDModules.end())
			{
				// FIX ME : CA2CT
				throw Exception( StringFormat::As( _T("%s is already defined."), id.c_str() ) );
			}

			m_IDModules[ id ] = module;
		}
	}

	void Unload( const _TCHAR * filename )
	{
		throw Exception( _T("Not supported yet") );
	}

	void UnloadAll()
	{
		m_IDModules.clear();
		m_LoadedModules.clear();
	}

	template<typename TPlugIn>
	boost::shared_ptr<TPlugIn> CreatePlugIn( const char *name )
	{
		IDContainer::iterator II = m_IDModules.find( std::string(name) );
		if (II == m_IDModules.end())
		{
			// FIX ME : CA2CT
			throw Exception( StringFormat::As( _T("%s is not defined in the loaded dlls."), name ) );
		}

		PlugInModulePtr module( II->second );
		return module->CreatePlugIn<TPlugIn>( name );
	}


private:

	typedef std::map< _tstring, PlugInModulePtr > ModuleContainer; 
	ModuleContainer m_LoadedModules;

	typedef std::map< std::string, PlugInModulePtr > IDContainer; 
	IDContainer m_IDModules;

};


typedef boost::shared_ptr<PlugInModuleManager> PlugInModuleManagerPtr;

}
}

#endif