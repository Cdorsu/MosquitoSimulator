#pragma once

#include "D3D11.h"
#include "Model.h"
#include "DefaultShader.h"
#include "WorldShader.h"
#include "C2DShader.h"
#include "C3DShader.h"
#include "SkyboxShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "TextureWindow.h"
#include "Camera.h"
#include "Text.h"
#include "Skybox.h"
#include "RenderTexture.h"
#include "LightView.h"
#include "LineManager.h"
#include "Mosquito.h"

class CGraphics sealed
{
private:
	struct SObjectToDraw
	{
		CModel * ModelToDraw;
		float * World;
		DirectX::XMMATRIX WorldMatrix;
		SObjectToDraw( CModel * Model, float* World )
			:ModelToDraw( Model ), World( World )
		{};
	};
public:
	static constexpr float CamNear = 0.1f;
	static constexpr float CamFar = 200.0f;
	static constexpr float FOV = 0.5f * ( FLOAT ) D3DX_PI;
	static constexpr UINT SHADOW_WIDTH = 1024;
	static constexpr UINT SHADOW_HEIGHT = 1024;
private:
	bool m_bFullscreen;
private:
	CD3D11 * m_D3D11;
	CDefaultShader * m_DefaultShader;
	CWorldShader * m_WorldShader;
	C2DShader * m_2DShader;
	C3DShader * m_3DShader;
	CSkyboxShader * m_SkyboxShader;
	CDepthShader * m_DepthShader;
	CDepthShader * m_DepthShaderEx;
	CShadowShader * m_ShadowShader;
	CCamera * m_FirstPersonCamera;
	CCamera * m_ThirdPersonCamera;
	CTextureWindow * m_DebugWindow;
	CLineManager * m_LineManager;
	CModel * m_Cube;
	CModel * m_Torus;
	CModel * m_Ground;
	CText * m_FPSText;
	CText * m_FrameTimeText;
	CText * m_ScoreText;
#if _DEBUG || DEBUG
	CText * m_DebugText;
#endif
	CSkybox * m_Skybox;
	CMosquito * m_Mosquito;

	/*Light*/
	CRenderTexture * m_Depthmap;
	CLightView * m_LightView;

	CLight * m_Light;
private:
	std::map<std::wstring, std::vector<SObjectToDraw>> m_mwvecObjectsToDraw;
	UINT m_iScore;
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
	void RenderUI( );
	void Render( );
	void RenderPlane( float* World );
	void RenderCube( float* World );
	void RenderTorus( float* World );
	void RenderLine( DirectX::XMFLOAT3 From, DirectX::XMFLOAT3 To, utility::SColor Color );
	void RenderPlayer( DirectX::XMFLOAT3 Position );
	void Shutdown( );
private:
	void RenderScene( );
public:
	inline void SetScore( UINT score )
	{
		m_iScore = score;
		if ( m_iScore > 9999 )
			m_iScore = 9999;
	}
public:
	inline void SwitchFullScreenState( )
	{
		m_D3D11->SwitchFullscreenState( m_bFullscreen ?
			( FALSE, m_bFullscreen = false ) : ( TRUE, m_bFullscreen = true ) );
	}
	inline void AddObjectToRenderList( std::wstring Name, CModel * Model, float* World )
	{
		m_mwvecObjectsToDraw[ Name ].emplace_back( Model, World );
		m_mwvecObjectsToDraw[ Name ][ m_mwvecObjectsToDraw[ Name ].size( ) - 1 ]
			.WorldMatrix = DirectX::XMMATRIX( World );
	}
public:
	inline CCamera * GetCamera( )
	{
		return m_ActiveCamera;
	}
public:
	inline CModel * GetTorus( )
	{
		return m_Torus;
	}
	inline CModel * GetCube( )
	{
		return m_Cube;
	}
public:
	inline void BeginScene( )
	{
		m_D3D11->EnableDefaultViewPort( );
		m_D3D11->EnableBackBuffer( );
		m_D3D11->BeginScene( );
	}
	inline void EndScene( )
	{
		RenderScene( );
		m_D3D11->EndScene( );
	}
};

