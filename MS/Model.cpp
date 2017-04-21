#include "Model.h"



CModel::CModel( )
{
	ZeroMemory( this, sizeof( CModel ) );
}

bool CModel::Initialize( ID3D11Device * device )
{
	m_Texture = new CTexture( );
	if ( !m_Texture->Initialize( device, L"2DArt\\Ana.jpeg" ) )
		return false;
	HRESULT hr;
	SVertex vertices[ ] =
	{
		SVertex(-1.0f, 1.0f, 0.5f, 0.0f,0.0f ),
		SVertex( 1.0f, 1.0f, 0.5f, 1.0f, 0.0f ),
		SVertex( 1.0f,-1.0f, 0.5f, 1.0f, 1.0f ),
		SVertex(-1.0f,-1.0f, 0.5f, 0.0f, 1.0f ),
	};
	m_VertexCount = sizeof( vertices ) / sizeof( vertices[ 0 ] );
	DWORD indices[ ] =
	{
		0, 1, 2,
		0, 2, 3,
	};
	m_IndexCount = sizeof( indices ) / sizeof( indices[ 0 ] );
	m_World = DirectX::XMMatrixIdentity( );
	D3D11_BUFFER_DESC buffDesc = { 0 };
	D3D11_SUBRESOURCE_DATA buffData = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = sizeof( vertices );
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffData.pSysMem = vertices;
	hr = device->CreateBuffer( &buffDesc, &buffData, &m_VertexBuffer );
	IFFAILED( hr, L"Couldn't create vertex buffer" );

	buffDesc.ByteWidth = sizeof( indices );
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	buffData.pSysMem = indices;
	hr = device->CreateBuffer( &buffDesc, &buffData, &m_IndexBuffer );
	IFFAILED( hr, L"Couldn't create index buffer" );

	return true;
}

void CModel::Render( ID3D11DeviceContext * context )
{
	static UINT Stride = sizeof( SVertex );
	static UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void CModel::Shutdown( )
{
	if ( m_Texture )
	{
		m_Texture->Shutdown( );
		delete m_Texture;
	}
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_IndexBuffer );
}

CModel::~CModel( )
{
}

void* CModel::operator new( size_t count )
{
	return _aligned_malloc( count,16 );
}

void CModel::operator delete ( void* object )
{
	_aligned_free( object );
}