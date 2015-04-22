#ifndef __STOP_WATCH_H__5_AUG_2010__
#define __STOP_WATCH_H__5_AUG_2010__

#include <windows.h>

namespace ProgrammingStudy {

class StopWatch
{
public:
    StopWatch()
    {
        QueryPerformanceCounter( (LARGE_INTEGER*) &m_StartTime);

        __int64 frequency;
        m_IsSupported = QueryPerformanceFrequency( (LARGE_INTEGER*) &frequency);
        if( m_IsSupported == TRUE )
        {
            m_Frequency = (double)frequency;
        }
    }
	virtual ~StopWatch(){}

    virtual double Elapsed()
    {
        if( m_IsSupported == FALSE )
            return 0.0;

        __int64 endTime;
        QueryPerformanceCounter( (LARGE_INTEGER*)&endTime);
        double elapsed = (double)( endTime - m_StartTime );
        return elapsed / m_Frequency;
   }

   virtual void Reset()
   {
        QueryPerformanceCounter( (LARGE_INTEGER*) &m_StartTime);
   }

protected:
    double m_Frequency;
    __int64 m_StartTime;
    BOOL    m_IsSupported;
};

class IntervalWatch : public StopWatch
{
public:
    double Elapsed()
    {
        if( m_IsSupported == FALSE )
            return 0.0;

        __int64 endTime;
        QueryPerformanceCounter( (LARGE_INTEGER*)&endTime);
        double elapsed = (double)( endTime - m_StartTime );

        m_StartTime = endTime;
        return elapsed / m_Frequency;
   }
};

class CorrectableWatch : public StopWatch
{
public:
    void CorrectWithReferenceTime( double referenceTime )
    {
        double delta = referenceTime - Elapsed();
        m_StartTime -= (long long)(delta * m_Frequency);
    }
};

}

#endif