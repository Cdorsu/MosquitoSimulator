#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\windowsIncludes.h"
#include "HRTimer.h"
#include "Graphics.h"

class CApplication sealed
{
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	CHRTimer m_Timer;
	UINT m_WindowWidth;
	UINT m_WindowHeight;
	UINT m_FPS;
	CGraphics * m_Graphics;
private:
	void InitWindow( HINSTANCE hInstance, bool bFullscreen );
public:
	CApplication( );
	CApplication( HINSTANCE hInstance, bool bFullscreen = true );
	~CApplication( );
public:
	void Run( );
public:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
};

