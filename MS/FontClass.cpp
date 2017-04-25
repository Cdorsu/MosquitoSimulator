#include "FontClass.h"



FontClass::FontClass( )
{
	ZeroMemory( this, sizeof( FontClass ) );
}

bool FontClass::Initialize( ID3D11Device* device, LPWSTR lpTexture, LPWSTR lpInfo, float Height )
{
	m_fFontHeight = Height;
	m_Texture = new CTexture( );
	if ( !m_Texture->Initialize( device, lpTexture ) )
		return false;

	m_Letters = new SLetter[ TotalLetters ];
	std::wifstream ifCitire( lpInfo );
	int index;
	wchar_t ch;
	for ( int i = 0; i < TotalLetters; ++i )
	{
		ifCitire >> index; // ignore it
		ifCitire >> ch;
		ifCitire >> m_Letters[ i ].left;
		ifCitire >> m_Letters[ i ].right;
		ifCitire >> m_Letters[ i ].width;
	}
	ifCitire.close( );


	return true;
}

void FontClass::Build( void * whereto, LPCSTR lpSentence, float x, float y )
{
	size_t nVertices = 0;
	size_t length = strlen( lpSentence );
	SVertex * vertices = ( SVertex* ) whereto;
	for ( size_t i = 0; i < length; ++i )
	{
		int charIndex = ( ( int ) lpSentence[ i ] - 32 );

		if ( charIndex <= 0 )
		{
			x += 3.0f;
			continue;
		}

		vertices[ nVertices ].Pos = DirectX::XMFLOAT3( x, y, 0.0f ); // Top left 
		vertices[ nVertices ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].left, 0.0f );
		nVertices++;

		vertices[ nVertices ].Pos = DirectX::XMFLOAT3( x + m_Letters[ charIndex ].width, y, 0.0f ); // Top right
		vertices[ nVertices ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].right, 0.0f );
		nVertices++;

		vertices[ nVertices ].Pos = DirectX::XMFLOAT3( x + m_Letters[ charIndex ].width, y - m_fFontHeight, 0.0f ); // Bottom right
		vertices[ nVertices ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].right, 1.0f );
		nVertices++;

		vertices[ nVertices ].Pos = DirectX::XMFLOAT3( x, y - m_fFontHeight, 0.0f ); // Bottom left
		vertices[ nVertices ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].left, 1.0f );
		nVertices++;

		x += m_Letters[ charIndex ].width + 1.0f;
	}
}

void FontClass::Shutdown( )
{
	if ( m_Texture )
	{
		m_Texture->Shutdown( );
		delete m_Texture;
		m_Texture = 0;
	}
}

FontClass::~FontClass( )
{
	Shutdown( );
}
