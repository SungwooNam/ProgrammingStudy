#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <tchar.h>
#include <string>
#include <stdarg.h>
#include <boost/cstdint.hpp>

namespace ProgrammingStudy
{

#ifdef _UNICODE 
    typedef std::wstring _tstring;
#else
    typedef std::string _tstring;
#endif

class Exception 
{
public:
    Exception( const _TCHAR* message ) : _Message( message ) {}
    Exception( const _tstring& message ) : _Message( message ) {}
    Exception( const Exception& ex ) : _Message( ex._Message ) {}
    virtual const _tstring& Message() const { return _Message; }

protected:
    _tstring _Message;
};

class StringFormat
{
public:
    static _tstring As( const _TCHAR *msg, ... )
	{
		const size_t EXP_BUFF_SIZE = 1023;
		_TCHAR sExpMsg[ EXP_BUFF_SIZE + 1];

		va_list argptr;
		va_start( argptr, msg);

#if _MSC_VER >= 1500
		_vsntprintf_s( sExpMsg, EXP_BUFF_SIZE, _TRUNCATE, msg, argptr );
#else

#endif
		va_end(argptr);

		return _tstring( sExpMsg );
	}
};

}

#endif