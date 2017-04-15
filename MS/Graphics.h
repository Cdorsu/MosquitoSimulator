#pragma once

#include "D3D11.h"

class CGraphics sealed
{
	CD3D11 * m_D3D11;
public:
	CGraphics( );
	CGraphics( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen = true );
	~CGraphics( );
public:
	inline void BeginScene( )
	{
		m_D3D11->BeginScene( );
	}
	inline void EndScene( )
	{
		m_D3D11->EndScene( );
	}
};

