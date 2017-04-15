#include "HRTimer.h"



CHRTimer::CHRTimer( )
{
	ZeroMemory( this, sizeof( CHRTimer ) );
}

void CHRTimer::StartTimer( )
{
	LARGE_INTEGER FrequencyCount;
	QueryPerformanceFrequency( &FrequencyCount );
	m_Frequency = ( float ) FrequencyCount.QuadPart;
	QueryPerformanceCounter( &FrequencyCount );
	m_StartTime = FrequencyCount.QuadPart;
	m_LastFrame = m_StartTime;
	m_Frames = 0;
}

void CHRTimer::Frame( )
{
	LARGE_INTEGER Count;
	QueryPerformanceCounter( &Count );
	m_CurrentTime = (Count.QuadPart - m_StartTime) / m_Frequency;
	m_Frames++;
	m_FrameTime = ( float ) ( Count.QuadPart - m_LastFrame ) / m_Frequency;
	m_LastFrame = Count.QuadPart;
}

CHRTimer::~CHRTimer( )
{
}
