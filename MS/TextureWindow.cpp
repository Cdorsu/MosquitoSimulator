#include "TextureWindow.h"



CTextureWindow::CTextureWindow( )
{
	ZeroMemory( this, sizeof( CTextureWindow ) );
}

bool CTextureWindow::Initialize( ID3D11Device * device, LPWSTR lpTexturePath,
	UINT WindowWidth, UINT WindowHeight, UINT ImageWidth, UINT ImageHeight )
{
	HRESULT hr;
	m_fPreviousX = -69;
	m_fPreviousY = -69;
	m_fWindowWidth = ( FLOAT ) WindowWidth;
	m_fWindowHeight = ( FLOAT ) WindowHeight;
	m_fImageWidth = ( FLOAT ) ImageWidth;
	m_fImageHeight = ( FLOAT ) ImageHeight;

	if ( lpTexturePath != L"" )
	{
		m_Texture = new CTexture( );
		m_Texture->Initialize( device, lpTexturePath );
	}

	DWORD indices[ ] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC buffDesc = { 0 };
	D3D11_SUBRESOURCE_DATA buffData = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof( SVertex ) * 4;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer( &buffDesc, nullptr, &m_VertexBuffer );
	IFFAILED( hr, L"Couldn't create a vertex buffer" );

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof( DWORD ) * 6;
	buffData.pSysMem = indices;
	
	hr = device->CreateBuffer( &buffDesc, &buffData, &m_IndexBuffer );
	IFFAILED( hr, L"Couldn't create a index buffer" );

	return true;
}

void CTextureWindow::SetShaders( ID3D11DeviceContext * context )
{
	static UINT Stride = sizeof( SVertex );
	static UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void CTextureWindow::Render( ID3D11DeviceContext * context, UINT PosX, UINT PosY )
{
	if ( m_fPreviousX == ( FLOAT ) PosX && m_fPreviousY == ( FLOAT ) PosY )
	{
		SetShaders( context );
		return;
	}
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	m_fPreviousX = ( FLOAT ) PosX;
	m_fPreviousY = ( FLOAT ) PosY;
	float left = ( m_fWindowWidth / 2 * -1.0f ) + PosX;
	float top = ( m_fWindowHeight / 2 ) - PosY;
	float right = left + m_fImageWidth;
	float bottom = top - m_fImageHeight;
	
	SVertex Vertices[] = 
	{
		SVertex( left,top, 0.0f,0.0f,0.0f ),
		SVertex( right,top, 0.0f,1.0f,0.0f ),
		SVertex( right,bottom, 0.0f,1.0f,1.0f ),
		SVertex( left,bottom, 0.0f, 0.0f,1.0f ),
	};
	hr = context->Map( m_VertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	//MappedResource.pData = Vertices;
	memcpy( MappedResource.pData, Vertices, sizeof( Vertices ) );
	context->Unmap( m_VertexBuffer, 0 );
	SetShaders( context );
}

void CTextureWindow::Shutdown( )
{
	if ( m_Texture )
	{
		m_Texture->Shutdown( );
		delete m_Texture;
		m_Texture = 0;
	}
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_IndexBuffer );
}

CTextureWindow::~CTextureWindow( )
{
	Shutdown( );
}
