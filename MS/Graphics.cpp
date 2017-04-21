#include "Graphics.h"



CGraphics::CGraphics( )
{
	ZeroMemory( this, sizeof( CGraphics ) );
}

bool CGraphics::Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen, CInput * Input )
{
	m_Input = Input;
	m_D3D11 = new CD3D11( );
	if ( !m_D3D11->Initialize( hWnd, WindowWidth, WindowHeight, 0.1f, 100.0f, bFullscreen ) )
		return false;

	m_DefaultShader = new CDefaultShader( );
	if ( !m_DefaultShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_WorldShader = new CWorldShader( );
	if ( !m_WorldShader->Initialize( m_D3D11->GetDevice( ) ) )
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
}

CGraphics::~CGraphics( )
{
	if ( m_Triangle )
	{
		m_Triangle->Shutdown( );
		delete m_Triangle;
	}
	if ( m_Camera )
	{
		m_Camera->Shutdown( );
		delete m_Camera;
	}
	if ( m_WorldShader )
	{
		m_WorldShader->Shutdown( );
		delete m_WorldShader;
	}
	if ( m_DefaultShader )
	{
		m_DefaultShader->Shutdown( );
		delete m_DefaultShader;
	}
	if ( m_D3D11 )
	{
		m_D3D11->Shutdown( );
		delete m_D3D11;
	}
}
