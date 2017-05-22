#include "Graphics.h"



CGraphics::CGraphics( )
{
	ZeroMemory( this, sizeof( CGraphics ) );
}

bool CGraphics::Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen, CInput * Input )
{
	m_bFullscreen = bFullscreen;
	m_Input = Input;

	m_Input->addSpecialKey( DIK_V );

	m_D3D11 = new CD3D11( );
	if ( !m_D3D11->Initialize( hWnd, WindowWidth, WindowHeight, CamNear, CamFar, bFullscreen ) )
		return false;

	m_DefaultShader = new CDefaultShader( );
	if ( !m_DefaultShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_WorldShader = new CWorldShader( );
	if ( !m_WorldShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_2DShader = new C2DShader( );
	if ( !m_2DShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_SkyboxShader = new CSkyboxShader( );
	if ( !m_SkyboxShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_DepthShader = new CDepthShader( );
	if ( !m_DepthShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_DepthShaderEx = new CDepthShader( );
	if ( !m_DepthShaderEx->Initialize( m_D3D11->GetDevice( ), true ) )
		return false;
	m_ShadowShader = new CShadowShader( );
	if ( !m_ShadowShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;

	m_FirstPersonCamera = new CCamera( );
	if ( !m_FirstPersonCamera->InitializeFirstPersonCamera( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, FOV, CamNear, CamFar, m_Input ) )
		return false;
	m_ThirdPersonCamera = new CCamera( );
	if ( !m_ThirdPersonCamera->InitializeThirdPersonCamera( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorZero( ), DirectX::XMVectorSet( 0.0f, 3.0f, -3.0f, 1.0f ),
		10.0f, ( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, FOV, CamNear, CamFar, m_Input ) )
		return false;
	m_ActiveCamera = m_FirstPersonCamera;

	m_DebugWindow = new CTextureWindow( );
	if ( !m_DebugWindow->Initialize( m_D3D11->GetDevice( ), L"", WindowWidth, WindowHeight, 100, 100 ) )
		return false;

	m_Cube = new CModel( );
	if ( !m_Cube->Initialize( m_D3D11->GetDevice( ), L"Assets\\Cube.aba" ) )
		return false;
	m_Torus = new CModel( );
	if ( !m_Torus->Initialize( m_D3D11->GetDevice( ), L"Assets\\Torus.aba" ) )
		return false;
	m_Ground = new CModel( );
	if ( !m_Ground->Initialize( m_D3D11->GetDevice( ), L"Assets\\Ground.aba" ) )
		return false;
	m_Font = new FontClass( );
	if ( !m_Font->Initialize( m_D3D11->GetDevice( ), L"Font\\font.dds", L"Font\\font.txt", 16 ) )
		return false;
	m_Font01 = new FontClass( );
	if ( !m_Font01->Initialize( m_D3D11->GetDevice( ), L"Font\\font01.dds", L"Font\\font01.txt", 32 ) )
		return false;

	m_FPSText = new CText( );
	if ( !m_FPSText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		10, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
	m_FrameTimeText = new CText( );
	if ( !m_FrameTimeText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		20, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
#if _DEBUG || DEBUG
	m_DebugText = new CText( );
	if ( !m_DebugText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		300, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
#endif

	m_Skybox = new CSkybox( );
	if ( !m_Skybox->Initialize( m_D3D11->GetDevice( ), L"Assets\\Skymap.dds" ) )
		return false;
#if  !(_DEBUG || DEBUG)
	m_Mosquito = new CMosquito( );
	if ( !m_Mosquito->Initialize( m_D3D11->GetDevice( ), L"Assets\\ComplexModels\\Mosquito.abal" ) )
		return false;
#endif
	m_Depthmap = new CRenderTexture( );
	if ( !m_Depthmap->Initialize( m_D3D11->GetDevice( ), SHADOW_WIDTH, SHADOW_HEIGHT,
		0.1f, 1.0f, 1, ( FLOAT ) 1 / ( FLOAT ) 1 ) )
		return false;

	m_Light = new CLight( );
	m_Light->SetDiffuse( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_Light->SetAmbient( utility::SColor( 0.2f, 0.2f, 0.2f, 1.0f ) );
	m_Light->SetDirection( 0.0f, -0.5f, 0.5f );
	m_Light->SetSpecularColor( m_Light->GetDiffuse( ) );
	m_Light->SetSpecularPower( 128.0f );

	m_LightView = new CLightView( );
	m_LightView->SetLookAt( DirectX::XMVectorZero( ) );
	m_LightView->SetPosition( DirectX::XMVectorSet( 0.1f, 6.0f, 0.0f, 1.0f ) );
	m_LightView->SetAmbient( utility::SColor( 0.1f, 0.1f, 0.1f, 1.0f ) );
	m_LightView->SetDiffuse( utility::hexToRGB( 0xFFFFFF ) );
	m_LightView->SetSpecularColor( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_LightView->GenerateProjectionMatrix( FOV, ( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, CamNear, CamFar );
	m_LightView->GenerateViewMatrix( );

	m_FirstPersonCamera->SetPosition( DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );
	m_ThirdPersonCamera->SetDirection( DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );

	return true;
}

void CGraphics::Update( float fFrameTime, UINT FPS )
{
	static float Rotation = 0.0f;

	if ( m_Input->isSpecialKeyPressed( DIK_V ) )
		m_ActiveCamera = m_ActiveCamera == m_FirstPersonCamera ? m_ThirdPersonCamera : m_FirstPersonCamera;

	Rotation += fFrameTime * 0.2f;
	if ( Rotation >= 4 * ( FLOAT ) D3DX_PI )
		Rotation = 0.0f;

	m_FirstPersonCamera->Update( );
	m_FirstPersonCamera->ConstructFrustum( );
	m_ThirdPersonCamera->Update( );
	m_ThirdPersonCamera->ConstructFrustum( );
	m_Skybox->Update( m_ActiveCamera );
#if !(DEBUG || _DEBUG)
	m_Mosquito->UpdateWings( );
#endif
	m_Cube->Identity( );
	m_Cube->RotateY( -Rotation );
	m_Cube->Translate( 0.0f, 1.0f, 3.0f );

	m_Torus->Identity( );
	m_Torus->RotateX( ( FLOAT ) D3DX_PI / 2.f );
	m_Torus->RotateY( Rotation );
	m_Torus->Translate( 0.0f, 3.2f, 3.0f );

	m_Ground->Identity( );
	m_Ground->Scale( 50.f, 50.f, 50.f );
#if !(DEBUG || _DEBUG)
	m_Mosquito->Identity( );
#endif
	char buffer[ 10 ] = { 0 };
	sprintf_s( buffer, "FPS: %d", FPS );
	m_FPSText->Update( m_D3D11->GetImmediateContext( ), 0, 0, buffer );
	char buffer2[ 20 ] = { 0 };
	sprintf_s( buffer2, "Frame time: %.2lf", fFrameTime );
	m_FrameTimeText->Update( m_D3D11->GetImmediateContext( ), 0, m_FPSText->GetHeight( ), buffer2 );
#if DEBUG || _DEBUG
	char buffer3[ 300 ] = { 0 };
	sprintf_s( buffer3, "DEBUG MODE" );
	m_DebugText->Update( m_D3D11->GetImmediateContext( ), 0,
		m_FPSText->GetHeight( ) + m_FrameTimeText->GetHeight( ), buffer3 );
#endif
}

void CGraphics::Render( )
{
	m_Depthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_Depthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );

	m_D3D11->EnableBackFaceCulling( );

	m_Ground->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ), m_Ground->GetWorld( ),
		m_LightView, m_Ground->GetTexture( ) );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_LightView, m_Cube->GetTexture( ) );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_LightView, m_Torus->GetTexture( ) );

	m_D3D11->DisableCulling( );
#if !(DEBUG || _DEBUG)
	for ( UINT i = 0; i < m_Mosquito->GetNumberOfObjects( ); ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
	}
#endif

	m_D3D11->EnableBackFaceCulling( );

	m_D3D11->EnableBackBuffer( );
	m_D3D11->EnableDefaultViewPort( );
	BeginScene( );

	m_Ground->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ), m_Ground->GetWorld( ),
		m_ActiveCamera, m_Ground->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_ActiveCamera, m_Cube->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_ActiveCamera, m_Torus->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );
	
	m_D3D11->DisableCulling( );
#if !(DEBUG || _DEBUG)
	for ( UINT i = 0; i < m_Mosquito->GetNumberOfObjects( ); ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
			m_LightView );
	}
#endif
	

	m_D3D11->EnableDSLessEqual( );

	m_Skybox->Render( m_D3D11->GetImmediateContext( ) );
	m_SkyboxShader->Render( m_D3D11->GetImmediateContext( ), m_Skybox->GetIndexCount( ),
		m_Skybox->GetWorld( ), m_ActiveCamera, m_Skybox->GetTexture( ) );

	m_D3D11->EnableDefaultDSState( );


	m_FPSText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FPSText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FPSText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_FrameTimeText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FrameTimeText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FrameTimeText->GetTexture( ),
		utility::SColor( 1.0f, 0.0f, 0.0f, 1.0f ) );

#if DEBUG || _DEBUG
	m_DebugText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_DebugText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_DebugText->GetTexture( ),
		utility::SColor( 0.0f, 1.0f, 1.0f, 1.0f ) );
#endif
	EndScene( );
}

CGraphics::~CGraphics( )
{
	if ( m_LightView )
	{
		delete m_LightView;
		m_LightView = 0;
	}
	if ( m_Light )
	{
		delete m_Light;
		m_Light = 0;
	}
	if ( m_Depthmap )
	{
		m_Depthmap->Shutdown( );
		delete m_Depthmap;
		m_Depthmap = 0;
	}
	if ( m_Skybox )
	{
		m_Skybox->Shutdown( );
		delete m_Skybox;
		m_Skybox = 0;
	}
	if ( m_Mosquito )
	{
		m_Mosquito->Shutdown( );
		delete m_Mosquito;
		m_Mosquito = 0;
	}
#if DEBUG || _DEBUG
	if ( m_DebugText )
	{
		m_DebugText->Shutdown( );
		delete m_DebugText;
		m_DebugText = 0;
	}
#endif
	if ( m_FrameTimeText )
	{
		m_FrameTimeText->Shutdown( );
		delete m_FrameTimeText;
		m_FrameTimeText = 0;
	}
	if ( m_FPSText )
	{
		m_FPSText->Shutdown( );
		delete m_FPSText;
		m_FPSText = 0;
	}
	if ( m_Font01 )
	{
		m_Font01->Shutdown( );
		delete m_Font01;
		m_Font01 = 0;
	}
	if ( m_Font )
	{
		m_Font->Shutdown( );
		delete m_Font;
		m_Font = 0;
	}
	if ( m_Ground )
	{
		m_Ground->Shutdown( );
		delete m_Ground;
		m_Ground = 0;
	}
	if ( m_Torus )
	{
		m_Torus->Shutdown( );
		delete m_Torus;
		m_Torus = 0;
	}
	if ( m_Cube )
	{
		m_Cube->Shutdown( );
		delete m_Cube;
		m_Cube = 0;
	}
	if ( m_DebugWindow )
	{
		m_DebugWindow->Shutdown( );
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}
	if ( m_ThirdPersonCamera )
	{
		m_ThirdPersonCamera->Shutdown( );
		delete m_ThirdPersonCamera;
		m_ThirdPersonCamera = 0;
	}
	if ( m_FirstPersonCamera )
	{
		m_FirstPersonCamera->Shutdown( );
		delete m_FirstPersonCamera;
		m_FirstPersonCamera = 0;
	}
	if ( m_ShadowShader )
	{
		m_ShadowShader->Shutdown( );
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}
	if ( m_DepthShaderEx )
	{
		m_DepthShaderEx->Shutdown( );
		delete m_DepthShaderEx;
		m_DepthShaderEx = 0;
	}
	if ( m_DepthShader )
	{
		m_SkyboxShader->Shutdown( );
		delete m_SkyboxShader;
		m_SkyboxShader = 0;
	}
	if ( m_SkyboxShader )
	{
		m_SkyboxShader->Shutdown( );
		delete m_SkyboxShader;
		m_SkyboxShader = 0;
	}
	if ( m_2DShader )
	{
		m_2DShader->Shutdown( );
		delete m_2DShader;
		m_2DShader = 0;
	}
	if ( m_WorldShader )
	{
		m_WorldShader->Shutdown( );
		delete m_WorldShader;
		m_WorldShader = 0;
	}
	if ( m_DefaultShader )
	{
		m_DefaultShader->Shutdown( );
		delete m_DefaultShader;
		m_DefaultShader = 0;
	}
	if ( m_D3D11 )
	{
		m_D3D11->Shutdown( );
		delete m_D3D11;
		m_D3D11 = 0;
	}
}
