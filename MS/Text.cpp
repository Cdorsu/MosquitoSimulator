#include "Text.h"



CText::CText( )
{
	ZeroMemory( this, sizeof( CText ) );
}

bool CText::Initialize( ID3D11Device * device, FontClass * Font, size_t maxLength, float WindowWidth, float WindowHeight )
{
	m_Font = Font;
	m_MaxLength = maxLength;
	m_fWindowWidth = WindowWidth;
	m_fWindowHeight = WindowHeight;

	HRESULT hr;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = sizeof( FontClass::SVertex ) * 4 * maxLength;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_VertexBuffer );
	IFFAILED( hr, L"Couldn't create a vertex buffer" );

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	buffDesc.ByteWidth = sizeof( DWORD ) * 6 * maxLength;
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_IndexBuffer );

	m_fPreviousX = -69.f;
	m_fPreviousY = -69.f;
	m_lpLastMessage = "";

	return true;
}

void CText::Update( ID3D11DeviceContext * context, float x, float y, LPCSTR Message )
{
	if ( x == m_fPreviousX && y == m_fPreviousY && // if it's at the same position
		strcmp( Message, m_lpLastMessage ) == 0 ) // And it's the same text
		return; // We already updated it
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE Vertices, Indices;

	x = ( m_fWindowWidth / 2 * -1 ) + x;
	y = ( m_fWindowHeight / 2 ) - y;

	hr = context->Map( m_VertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &Vertices );
	if ( FAILED( hr ) )
		return;
	/*memset( MappedResource, 0, sizeof( FontClass::SVertex ) * 4 * m_MaxLength ); *//// Useless
	
	hr = context->Map( m_IndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &Indices );
	if ( FAILED( hr ) )
	{
		context->Unmap( m_VertexBuffer, 0 );
		return;
	}

	m_Font->Build( Vertices.pData, Indices.pData, m_VertexCount, m_IndexCount, Message, x, y );

	context->Unmap( m_IndexBuffer, 0 );

	context->Unmap( m_VertexBuffer, 0 );

}

void CText::Render( ID3D11DeviceContext * context )
{
	static UINT Stride = sizeof( FontClass::SVertex );
	static UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void CText::Shutdown( )
{
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_IndexBuffer );
}

CText::~CText( )
{
	Shutdown( );
}
