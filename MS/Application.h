#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\windowsIncludes.h"
#include "Graphics.h"
#include "Input.h"
#include "HRTimer.h"
#include "Physics.h"
#include "resource.h"

class CApplication sealed
{
	static constexpr UINT SecondsToWait = 5;
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	CHRTimer m_Timer;
	UINT m_WindowWidth;
	UINT m_WindowHeight;
	UINT m_FPS;
	CGraphics * m_Graphics = nullptr;
	CInput * m_Input = nullptr;
	CPhysics * m_Physics = nullptr;
	bool m_bShowMenu = true;
	bool m_bGameRunning = true;
	std::chrono::system_clock::time_point m_ReleaseTime;
private:
	bool InitWindow( HINSTANCE hInstance, bool bFullscreen );
public:
	CApplication( );
	~CApplication( );
public:
	bool Initialize( HINSTANCE hInstance, bool bFullscreen = true );
	void Run( );
	void Shutdown( );
public:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
};

