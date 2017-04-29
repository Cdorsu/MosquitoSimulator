#include "Graphics.h"



CGraphics::CGraphics( )
{
	ZeroMemory( this, sizeof( CGraphics ) );
}

bool CGraphics::Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen, CInput * Input )
{
	m_Input = Input;
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

	m_Camera = new CCamera( );
	if ( !m_Camera->Initialize( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorSet( 0.0f, 0.0f, -5.0f, 1.0f ),
		( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, 0.5f * ( FLOAT ) D3DX_PI, 0.1f, 100.0f, m_Input ) )
		return false;

	m_Cube = new CModel( );
	if ( !m_Cube->Initialize( m_D3D11->GetDevice( ), L"Assets\\Cube.aba" ) )
		return false;
	m_Torus = new CModel( );
	if ( !m_Torus->Initialize( m_D3D11->GetDevice( ), L"Assets\\Torus.aba" ) )
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

	m_Skybox = new CSkybox( );
	if ( !m_Skybox->Initialize( m_D3D11->GetDevice( ), L"Assets\\Skymap.dds" ) )
		return false;

	m_Light = new CLight( );
	m_Light->SetDiffuse( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_Light->SetAmbient( utility::SColor( 0.2f, 0.2f, 0.2f, 1.0f ) );
	m_Light->SetDirection( 0.0f, 0.0f, 1.0f );
	m_Light->SetSpecularColor( m_Light->GetDiffuse( ) );
	m_Light->SetSpecularPower( 128.0f );

	return true;
}

void CGraphics::Update( float fFrameTime, UINT FPS )
{
	static float Rotation = 0.0f;

	Rotation += fFrameTime * 0.2f;
	if ( Rotation >= 4 * ( FLOAT ) D3DX_PI )
		Rotation = 0.0f;

	m_Camera->Update( );
	m_Skybox->Update( m_Camera );

	m_Cube->Identity( );
	//m_Cube->RotateY( -Rotation );

	m_Torus->Identity( );
	//m_Torus->RotateY( Rotation / 2 );
	m_Torus->Scale( 2.f, 2.f, 2.f );

	char buffer[ 10 ] = { 0 };
	sprintf_s( buffer, "FPS: %d", FPS );
	m_FPSText->Update( m_D3D11->GetImmediateContext( ), 0, 0, buffer );
	char buffer2[ 20 ] = { 0 };
	sprintf_s( buffer2, "Frame time: %.2lf", fFrameTime );
	m_FrameTimeText->Update( m_D3D11->GetImmediateContext( ), 0, m_FPSText->GetHeight( ), buffer2 );

}

void CGraphics::Render( )
{
	m_Camera->Render( );

	m_D3D11->EnableBackFaceCulling( );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_WorldShader->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_Camera, m_Cube->GetTexture( ), m_Cube->GetSpecularMap( ), m_Cube->GetBumpmap( ), m_Light );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_WorldShader->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_Camera, m_Torus->GetTexture( ), m_Torus->GetSpecularMap( ), m_Torus->GetBumpmap( ), m_Light );

	m_D3D11->DisableCulling( );

	m_D3D11->EnableDSLessEqual( );

	m_Skybox->Render( m_D3D11->GetImmediateContext( ) );
	m_SkyboxShader->Render( m_D3D11->GetImmediateContext( ), 36, m_Skybox->GetWorld( ), m_Camera, m_Skybox->GetTexture( ) );

	m_D3D11->EnableDefaultDSState( );

	m_FPSText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FPSText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FPSText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_FrameTimeText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FrameTimeText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FrameTimeText->GetTexture( ),
		utility::SColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
}

CGraphics::~CGraphics( )
{
	if ( m_Light )
	{
		delete m_Light;
		m_Light = 0;
	}
	if ( m_Skybox )
	{
		m_Skybox->Shutdown( );
		delete m_Skybox;
		m_Skybox = 0;
	}
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
	if ( m_Camera )
	{
		m_Camera->Shutdown( );
		delete m_Camera;
		m_Camera = 0;
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
