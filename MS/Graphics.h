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
#include "SunShadowShader.h"
#include "AddTexturesShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "TextureWindow.h"
#include "Camera.h"
#include "Text.h"
#include "Skybox.h"
#include "RenderTexture.h"
#include "LightView.h"
#include "SunLightView.h"
#include "LineManager.h"
#include "Mosquito.h"

#define USE_LIGHT

class CGraphics sealed
{
public:
#pragma warning (push)
#pragma warning (disable : 4369)
	enum class EMenuOption
	{
		StartGame, QuitGame, Nothing = 0xffffffff
	};
#pragma warning (pop)
private:
	struct SObjectToDraw
	{
		float * World;
		DirectX::XMFLOAT4X4 _4x4fWorld;
		bool bRenderDepthmap;
		bool bRenderBackBuffer;
		bool bRenderSunDepthmap;
		SObjectToDraw( float* World, bool RenderCam, bool RenderLightShadow, bool RenderSunShadow )
			: World( World ), bRenderDepthmap( RenderLightShadow ), bRenderBackBuffer( RenderCam ),
			bRenderSunDepthmap( RenderSunShadow )
		{
			DirectX::XMStoreFloat4x4( &_4x4fWorld, DirectX::XMMATRIX( World ) );
		};
	};
	struct SAdditionalInfoFromPhysicsEngine
	{
		DirectX::XMFLOAT3 PlayerDirection = DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
		bool bAnimateWings = false;
	};
public:
	static constexpr float CamNear = 0.1f;
	static constexpr float CamFar = 200.0f;
	static constexpr float FOV = 0.5f * ( FLOAT ) D3DX_PI;
	static constexpr float SunDistanceToCamera = 100.f; // You can play
	static constexpr float SunInFrontOfCamera = 10.f; //				with these values
	static constexpr float SunWidthHeight = 40.f; //										to get different results
	static constexpr UINT SHADOW_WIDTH = 1024;
	static constexpr UINT SHADOW_HEIGHT = 1024;
	static constexpr UINT DistanceFromRightWindowLeftMap = 120;
	static constexpr UINT DistanceFromTopToTopMap = 20;
	static constexpr UINT MapWidth = 100;
	static constexpr UINT MapWidthOver2 = 50;
	static constexpr UINT MenuOutlineWidth = 512;
	static constexpr UINT MenuOutlineHeight = 512;
#if DEBUG || _DEBUG
	static constexpr UINT DebugWindowWidthHeight = 100;
#endif
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
	CSunShadowShader * m_SunShadowShader;
	CAddTexturesShader * m_AddTexturesShader;
	CHorizontalBlurShader * m_HorizontalBlurShader;
	CVerticalBlurShader * m_VerticalBlurShader;
	CCamera * m_FirstPersonCamera;
	CCamera * m_ThirdPersonCamera;
#if _DEBUG || DEBUG
	CTextureWindow * m_DebugWindow;
#endif
	CTextureWindow * m_FullscreenWindow;
	CTextureWindow * m_MapWindow;
	CTextureWindow * m_PlayerWindow;
	CTextureWindow * m_CheckpointWindow;
	CLineManager * m_LineManager;
	CModel * m_Cube;
	CModel * m_Torus;
	CModel * m_Ground;
	CModel * m_Window;
	CModel * m_Ceil;
	CModel * m_LightBulb;
	CModel * m_Table;
	CModel * m_Chair;
	CModel * m_Doggo;
	CText * m_FPSText;
	CText * m_FrameTimeText;
	CText * m_ScoreText;
#if _DEBUG || DEBUG
	CText * m_DebugText;
#endif
	CSkybox * m_Skybox;
	CMosquito * m_Mosquito;

	/*Light*/
	CRenderTexture * m_LightDepthmap;
	CLightView * m_LightView;

	/*Sun*/
	CRenderTexture * m_SunDepthmap;
	CSunLightView * m_SunLightView;

	/*Textures to add*/
	CRenderTexture * m_SceneWithSunLight;
	CRenderTexture * m_SceneWithLight;

private:
	CTextureWindow * m_MenuOutline;
	CTextureWindow * m_Cursor;
	std::vector<CText*> m_vecMenuTexts;
	struct
	{
		float m_fCursorX;
		float m_fCursorY;
	};
	UINT m_MenuSelected;

private:
	std::map<std::wstring, std::vector<SObjectToDraw>> m_mwvecObjectsToDraw;
	SAdditionalInfoFromPhysicsEngine m_AdditionalPhysicsInfo;
protected:
	UINT m_WindowWidth;
	UINT m_WindowHeight;
	UINT m_iScore;
private: // Fonts
	FontClass * m_Font;
	FontClass * m_Font01;
private: // Just pointers, do not create, do not destroy
	CCamera * m_ActiveCamera;
private: // Do not create, do not destroy, just take it from an upper level (CApplication)
	CInput * m_Input;
private:
	float m_CheckpointX;
	float m_CheckpointZ;
	float m_PlayerX;
	float m_PlayerZ;
public:
	CGraphics( );
	~CGraphics( );
public:
	bool Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen = true, CInput * Input = nullptr );
	void Update( float fFrameTime, UINT FPS );
	void RenderMenu( float fFrameTime, UINT FPS );
	void RenderUI( );
	void Render( );
	void RenderGround( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderCube( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderTorus( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderLine( DirectX::XMFLOAT3 From, DirectX::XMFLOAT3 To, utility::SColor Color );
	void RenderPlayer( DirectX::XMFLOAT3 Position, float * World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderWall( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderCeiling( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderLightBulb( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderTable( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderChair( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void RenderDoggo( float* World,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
	void Shutdown( );
private:
	void RenderScene( bool bRenderUI = true );
	void RenderMosquito( float * World, bool drawtoback,
		float minX = 0, float minY = 0, float minZ = 0,
		float maxX = 0, float maxY = 0, float maxZ = 0 );
public:
	inline void SetScore( UINT score )
	{
		m_iScore = score;
		if ( m_iScore > 9999 )
			m_iScore = 9999;
	}
	inline void SetPlayerDirection( DirectX::XMFLOAT3 PlayerDirection)
	{
		m_AdditionalPhysicsInfo.PlayerDirection = PlayerDirection;
	}
	inline void SetUserTouchesTheGround( bool bTouchesTheGround )
	{
		m_AdditionalPhysicsInfo.bAnimateWings = !bTouchesTheGround;
	}
public:
	inline void SwitchFullScreenState( )
	{
		m_D3D11->SwitchFullscreenState( m_bFullscreen ?
			( FALSE, m_bFullscreen = false ) : ( TRUE, m_bFullscreen = true ) );
	}
	inline void AddObjectToRenderList( std::wstring Name, float* World,
		bool RenderShadow = true, bool RenderCam = true, bool RenderSunShadow = true )
	{	
		m_mwvecObjectsToDraw[ Name ].emplace_back(World, RenderCam, RenderShadow, RenderSunShadow );
	}
public:
	inline CCamera * GetCamera( )
	{
		return m_FirstPersonCamera;
	}
	inline EMenuOption GetMenuSelection( )
	{
		return static_cast< EMenuOption >( m_MenuSelected );
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
	inline CModel * GetTable( )
	{
		return m_Table;
	}
	inline CModel * GetChair( )
	{
		return m_Chair;
	}
	inline CModel * GetDoggo( )
	{
		return m_Doggo;
	}
	inline std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> GetPlayerAABB( )
	{
		std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3> AABB;
		AABB.first = m_Mosquito->GetMinAABB( );
		AABB.second = m_Mosquito->GetMaxAABB( );
		return AABB;
	}
public:
	inline void BeginFrame( )
	{
		m_D3D11->EnableDefaultViewPort( );
		m_D3D11->EnableBackBuffer( );
		m_D3D11->DisableCulling( );
		m_D3D11->BeginScene( );
	}
	inline void EndFrame( )
	{
		m_D3D11->EndScene( );
	}
	inline void BeginScene( )
	{
		m_D3D11->EnableDefaultViewPort( );
		m_D3D11->EnableBackBuffer( );
		m_LightView->ConstructFrustum( );
		m_ActiveCamera->ConstructFrustum( );
		m_D3D11->BeginScene( );
	}
	inline void EndScene( bool bRenderUI = true )
	{
		RenderScene( bRenderUI );
		m_D3D11->EndScene( );
	}
};

