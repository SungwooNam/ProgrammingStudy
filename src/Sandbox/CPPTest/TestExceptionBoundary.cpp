#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <stdio.h>
#include <windows.h>
#include <boost\shared_ptr.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestExceptionBoundary )

typedef boost::shared_ptr<void> SharedHandle;

class EmergencyMonitor
{
public:
	EmergencyMonitor()
	{
		SharedHandle m_Thread = SharedHandle(
			CreateThread( NULL, 0, MonitorThread, this, CREATE_SUSPENDED, NULL ),
			CloseHandle );

		SharedHandle m_Terminate = SharedHandle(
			CreateEvent( NULL, FALSE, FALSE, NULL ),
			CloseHandle );
	}

	~EmergencyMonitor()
	{
		SetEvent( m_Terminate.get() );
		WaitForSingleObject( m_Thread.get(), INFINITE );
	}

	bool IsThereSmoke() const { return false; }
	bool IsThereFire() const { return true; }

private:
	SharedHandle m_Thread, m_Terminate;
	bool IsTerminating() 
	{ 
		return WaitForSingleObject( m_Terminate.get(), 10 ) == WAIT_OBJECT_0;
	}

	static DWORD __stdcall MonitorThread( LPVOID arg )
	{
		EmergencyMonitor* monitor = (EmergencyMonitor*)(arg);

		while( !monitor->IsTerminating() )
		{
			if( monitor->IsThereSmoke() || monitor->IsThereFire() )
			{
				// throw std::exception( "Smoke or Fire!" );
			}
		}
		
		return 0;
	}
};

BOOST_AUTO_TEST_CASE( ExceptionCantCrossThreadBoundary )
{
	EmergencyMonitor monitor;

	Sleep( 100 );
}

BOOST_AUTO_TEST_SUITE_END()