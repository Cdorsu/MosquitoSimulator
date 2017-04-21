#include "Texture.h"



CTexture::CTexture( )
{
	ZeroMemory( this, sizeof( CTexture ) );
}

bool CTexture::Initialize( ID3D11Device * device, LPWSTR lpFilepath )
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile( device,
		lpFilepath, // Get Texture from here
		NULL, // No furhter description
		nullptr, // No threadpump
		&m_Texture,
		nullptr ); // No thread pump -> NULL
	IFFAILED( hr, L"Couldn't load a texture" );

	return true;
}

void CTexture::Shutdown( )
{
	SAFE_RELEASE( m_Texture );
}

CTexture::~CTexture( )
{
	Shutdown( );
}
