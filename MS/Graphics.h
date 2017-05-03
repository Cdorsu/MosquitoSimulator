#pragma once

#include "D3D11.h"
#include "Model.h"
#include "DefaultShader.h"
#include "WorldShader.h"
#include "C2DShader.h"
#include "SkyboxShader.h"
#include "TextureWindow.h"
#include "Camera.h"
#include "Text.h"
#include "Skybox.h"
#include "RenderTexture.h"

class CGraphics sealed
{
public:
	static constexpr float CamNear = 1.0f;
	static constexpr float CamFar = 100.0f;
	static constexpr float FOV = 0.5f * ( FLOAT ) D3DX_PI;
private:
	CD3D11 * m_D3D11;
	CDefaultShader * m_DefaultShader;
	CWorldShader * m_WorldShader;
	C2DShader * m_2DShader;
	CSkyboxShader * m_SkyboxShader;
	CCamera * m_FirstPersonCamera;
	CCamera * m_ThirdPersonCamera;
	CModel * m_Cube;
	CModel * m_Torus;
	CText * m_FPSText;
	CText * m_FrameTimeText;
	CSkybox * m_Skybox;

	CLight * m_Light;
private: // Fonts
	FontClass * m_Font;
	FontClass * m_Font01;
private: // Just pointers, do not create, do not destroy
	CCamera * m_ActiveCamera;
private: // Do not create, do not destroy, just take it from an upper level (CApplication)
	CInput * m_Input;
public:
	CGraphics( );
	~CGraphics( );
public:
	bool Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen = true, CInput * Input = nullptr );
	void Update( float fFrameTime, UINT FPS );
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

