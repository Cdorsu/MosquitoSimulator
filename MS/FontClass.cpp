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

void FontClass::Build( _Out_ void * whereto, _Out_ void * Indices, _Out_ UINT& VertexCount, _Out_ UINT& IndexCount,
	_In_ LPCSTR lpSentence, _In_ float x, _In_ float y, _Out_ float& Width )
{
	size_t length = strlen( lpSentence );
	VertexCount = 0;
	IndexCount = 0;
	Width = x;
	SVertex * vertices = ( SVertex* ) whereto;
	DWORD * indices = ( DWORD* ) Indices;
	for ( size_t i = 0; i < length; ++i )
	{
		int charIndex = ( ( int ) lpSentence[ i ] - 32 );

		if ( charIndex <= 0 )
		{
			x += 3.0f;
			continue;
		}

		// First triangle
		vertices[ VertexCount ].Pos = DirectX::XMFLOAT3( x, y, 0.0f ); // Top left 
		vertices[ VertexCount ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].left, 0.0f );
		indices[ IndexCount++ ] = VertexCount; // 0
		VertexCount++;

		vertices[ VertexCount ].Pos = DirectX::XMFLOAT3( x + m_Letters[ charIndex ].width, y, 0.0f ); // Top right
		vertices[ VertexCount ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].right, 0.0f );
		indices[ IndexCount++ ] = VertexCount; // 1
		VertexCount++;

		vertices[ VertexCount ].Pos = DirectX::XMFLOAT3( x + m_Letters[ charIndex ].width, y - m_fFontHeight, 0.0f ); // Bottom right
		vertices[ VertexCount ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].right, 1.0f );
		indices[ IndexCount++ ] = VertexCount; // 2
		VertexCount++;

		// Done first triangle
		indices[ IndexCount ] = indices[ IndexCount - 3 ];
		indices[ IndexCount + 1 ] = indices[ IndexCount - 1 ];
		IndexCount += 2;

		vertices[ VertexCount ].Pos = DirectX::XMFLOAT3( x, y - m_fFontHeight, 0.0f ); // Bottom left
		vertices[ VertexCount ].Tex = DirectX::XMFLOAT2( m_Letters[ charIndex ].left, 1.0f );
		indices[ IndexCount++ ] = VertexCount; // 3
		VertexCount++;

		x += m_Letters[ charIndex ].width + 1.0f;
	}
	Width = x - Width;
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
