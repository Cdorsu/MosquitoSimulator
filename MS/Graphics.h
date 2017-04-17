#pragma once

#include "D3D11.h"
#include "Model.h"
#include "DefaultShader.h"

class CGraphics sealed
{
	CD3D11 * m_D3D11 = nullptr;
	CModel * m_Triangle = nullptr;
	CDefaultShader * m_DefaultShader = nullptr;
public:
	CGraphics( );
	~CGraphics( );
public:
	bool Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen = true );
	void Render( );
	void Shutdown( );
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

