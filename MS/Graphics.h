#pragma once

#include "D3D11.h"
#include "Model.h"
#include "DefaultShader.h"
#include "WorldShader.h"
#include "Camera.h"

class CGraphics sealed
{
	CD3D11 * m_D3D11;
	CDefaultShader * m_DefaultShader;
	CWorldShader * m_WorldShader;
	CCamera * m_Camera;
	CModel * m_Triangle;

	CInput * m_Input;
public:
	CGraphics( );
	~CGraphics( );
public:
	bool Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen = true, CInput * Input = nullptr );
	void Update( float fFrameTime );
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

