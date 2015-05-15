#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include "Common\Exception.hpp"
#include <boost/serialization/singleton.hpp>

using namespace std;
using namespace ProgrammingStudy;

BOOST_AUTO_TEST_SUITE( TestDesignPatternSingleton )

namespace SimpleLogger {

class FileLogger {
public:
	FileLogger() : m_MsgCount(0) {}
	void Log( const TCHAR* msg ) { 
		// write msg to file  
		m_MsgCount++;
	}
	int GetMsgCount() const { return m_MsgCount; }
private:
	int m_MsgCount;
};

typedef boost::serialization::singleton<FileLogger> TheFileLogger;

}

BOOST_AUTO_TEST_CASE( ShouldTheSimpleLogger )
{  
	using namespace SimpleLogger;
	int count = 0;

	{
		FileLogger& logger( TheFileLogger::get_mutable_instance() );
		count = logger.GetMsgCount();
		logger.Log( _T("Hello World") );
		BOOST_CHECK_EQUAL( logger.GetMsgCount(), count+1 );
	}

	{
		const FileLogger& logger( TheFileLogger::get_const_instance() );
		BOOST_CHECK_EQUAL( logger.GetMsgCount(), count+1 );
	}
}

BOOST_AUTO_TEST_SUITE_END()