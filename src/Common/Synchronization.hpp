#ifndef __SYNCHRONIZATION_HPP_2015_05_27__
#define __SYNCHRONIZATION_HPP_2015_05_27__

#include "Exception.hpp"

namespace ProgrammingStudy
{

class Event {
public:
	Event( bool manual, bool initial ) { 
		if( (m_Event = ::CreateEvent( NULL, manual, initial, NULL ))== NULL )  {
			throw Exception( StringFormat::As(  
				_T("Can't create event manual %d, initial %d"), manual, initial ));
		}
	}
	~Event() { CloseHandle(m_Event); }

	void Set() { 
		if( ::SetEvent( m_Event ) == FALSE ) {
			throw Exception( StringFormat::As(  _T("Can't set event")));
		}
	}
	
	bool TryWaitFor( double second ) { 
		DWORD ret = WaitForSingleObject( m_Event, (ULONG)(second * 1000) );
		if( ret == WAIT_OBJECT_0 ) 
			return true;
		return false;
	}

	HANDLE GetHandle() { return m_Event; }
private:
	HANDLE m_Event;

	Event(Event const&);
    Event& operator=(Event const&);
};


class Semaphore {
public:
	Semaphore( int initial, int maximum ) { 
		if( (m_Sem = ::CreateSemaphore( NULL, initial, maximum, NULL ))== NULL )  {
			throw Exception( StringFormat::As( 
				_T("Can't create semaphore with initial %d, maximum %d"), initial, maximum ));
		}
	}
	~Semaphore() { CloseHandle(m_Sem); }

	void Release() { 
		if( ReleaseSemaphore( m_Sem, 1, NULL ) == FALSE ) {
			throw Exception( StringFormat::As( 
				_T("Can't release semaphore") ));
		}
	}

	bool TryWaitFor( double second ) { 
		DWORD ret = WaitForSingleObject( m_Sem, (ULONG)(second * 1000) );
		if( ret == WAIT_OBJECT_0 ) 
			return true;
		return false;
	}

	HANDLE GetHandle() { return m_Sem; }
private:
	HANDLE m_Sem;

	Semaphore(Semaphore const&);
    Semaphore& operator=(Semaphore const&);
};


}

#endif