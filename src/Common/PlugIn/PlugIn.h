#ifndef __PLUG_IN_H__
#define __PLUG_IN_H__

#include "IPlugIn.h"
#include <Shlwapi.h>

#ifdef PLUGIN_EXPORTS
#define LIBSPECS extern "C" __declspec(dllexport)
#else
#define LIBSPECS extern "C" __declspec(dllimport)
#endif


LIBSPECS HRESULT DllGetVersion( DLLVERSIONINFO *pvdi );
LIBSPECS ProgrammingStudy::PlugIn::IPlugInFactory* CPI_CreateFactory();

#endif