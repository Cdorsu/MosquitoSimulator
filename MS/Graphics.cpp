#include "Graphics.h"



CGraphics::CGraphics( )
{
	ZeroMemory( this, sizeof( CGraphics ) );
}

bool CGraphics::Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen )
{
	m_D3D11 = new CD3D11( );
	if ( !m_D3D11->Initialize( hWnd, WindowWidth, WindowHeight, bFullscreen ) )
		return false;

	m_DefaultShader = new CDefaultShader( );
	if ( !m_DefaultShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;

	m_Triangle = new CModel( );
	if ( !m_Triangle->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	return true;
}

void CGraphics::Render( )
{
	m_Triangle->Render( m_D3D11->GetImmediateContext( ) );
	m_DefaultShader->Render( m_D3D11->GetImmediateContext( ), m_Triangle->GetIndexCount( ) );
}

CGraphics::~CGraphics( )
{
	if ( m_DefaultShader )
	{
		m_DefaultShader->Shutdown( );
		delete m_DefaultShader;
	}
	if ( m_Triangle )
	{
		m_Triangle->Shutdown( );
		delete m_Triangle;
	}
	if ( m_D3D11 )
	{
		m_D3D11->Shutdown( );
		delete m_D3D11;
	}
}
