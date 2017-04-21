#pragma once

#include "common\windowsIncludes.h"

class CHRTimer sealed
{
	UINT m_Frames;
	LONGLONG m_StartTime;
	LONGLONG m_LastFrame;
	float m_Frequency;
	float m_CurrentTime;
	float m_FrameTime;
public:
	CHRTimer( );
	~CHRTimer( );
public:
	inline float GetFrameTime( ) { return m_FrameTime; };
	inline float GetTime( ) { return m_CurrentTime; };
	inline UINT GetFramesPerSecond( ) { return m_Frames; };
public:
	void StartTimer( );
	void Frame( );
};

