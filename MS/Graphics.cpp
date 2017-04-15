#include "Graphics.h"



CGraphics::CGraphics( )
{
	ZeroMemory( this, sizeof( CGraphics ) );
}

CGraphics::CGraphics( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen )
{
	m_D3D11 = new CD3D11( hWnd, WindowWidth, WindowHeight, !bFullscreen );
}

CGraphics::~CGraphics( )
{
	if ( m_D3D11 )
		delete m_D3D11;
}
