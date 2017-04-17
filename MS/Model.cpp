#include "Model.h"



CModel::CModel( )
{
	ZeroMemory( this, sizeof( CModel ) );
}

bool CModel::Initialize( ID3D11Device * device )
{
	HRESULT hr;
	SVertex vertices[ ] =
	{
		SVertex( 0.0f,0.5f,0.5f ),
		SVertex( 0.5f,-0.5f,0.5f ),
		SVertex( -0.5f,-0.5f,0.5f )
	};
	m_VertexCount = sizeof( vertices ) / sizeof( vertices[ 0 ] );
	DWORD indices[] =
	{
		0, 1, 2,
	};
	m_IndexCount = sizeof( indices ) / sizeof( indices[ 0 ] );
	D3D11_BUFFER_DESC buffDesc = { 0 };
	D3D11_SUBRESOURCE_DATA buffData = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = sizeof( SVertex ) * 3;
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
	UINT Stride = sizeof( SVertex );
	UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void CModel::Shutdown( )
{
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_IndexBuffer );
}

CModel::~CModel( )
{
	Shutdown( );
}