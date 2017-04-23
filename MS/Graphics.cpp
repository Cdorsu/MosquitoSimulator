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

	m_Image = new CTextureWindow( );
	if ( !m_Image->Initialize( m_D3D11->GetDevice( ), L"2DArt\\Ana.jpeg", WindowWidth, WindowHeight, 300, 300 ) )
		return false;

	m_Camera = new CCamera( );
	if ( !m_Camera->Initialize( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorSet( 0.0f, 0.0f, -5.0f, 1.0f ),
		( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, 0.5f * ( FLOAT ) D3DX_PI, 0.1f, 100.0f, m_Input ) )
		return false;

	m_Triangle = new CModel( );
	if ( !m_Triangle->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	return true;
}

void CGraphics::Update( float fFrameTime )
{
	m_Camera->Update( );
}

void CGraphics::Render( )
{
	m_Camera->Render( );

	m_Triangle->Render( m_D3D11->GetImmediateContext( ) );
	m_WorldShader->Render( m_D3D11->GetImmediateContext( ), m_Triangle->GetIndexCount( ), m_Triangle->GetWorld( ),
		m_Camera->GetView( ), m_Camera->GetProjection( ), m_Triangle->GetTexture( ) );

	m_Image->Render( m_D3D11->GetImmediateContext( ), 10, 10 );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_Image->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_Image->GetTexture( ) );
}

CGraphics::~CGraphics( )
{
	if ( m_Triangle )
	{
		m_Triangle->Shutdown( );
		delete m_Triangle;
		m_Triangle = 0;
	}
	if ( m_Camera )
	{
		m_Camera->Shutdown( );
		delete m_Camera;
		m_Camera = 0;
	}
	if ( m_Image )
	{
		m_Image->Shutdown( );
		delete m_Image;
		m_Image = 0;
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
