#include "LineManager.h"



CLineManager::CLineManager( )
{
}

bool CLineManager::Initialize( ID3D11Device * device )
{
	static HRESULT hr;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof( SVertex ) * 2;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_VertexBuffer );
	IFFAILED( hr, L"Couldn't create a vertex buffer" );
	return true;
}

void CLineManager::Render( ID3D11DeviceContext * context, DirectX::XMFLOAT3 From, DirectX::XMFLOAT3 To )
{
	static auto isEqual = []( DirectX::XMFLOAT3& First, DirectX::XMFLOAT3& Second )
		->bool
	{
		return First.x == Second.x && First.y == Second.y && First.z == Second.z;
	};
	if ( isEqual( From, m_3fLastLineFrom ) &&
		isEqual( To, m_3fLastLineTo ) )
	{
		RenderBuffers( context );
		return;
	}
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	m_3fLastLineFrom = From;
	m_3fLastLineTo = To;
	hr = context->Map( m_VertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SVertex* ) MappedResource.pData )[ 0 ].Position = From;
	( ( SVertex* ) MappedResource.pData )[ 1 ].Position = To;
	context->Unmap( m_VertexBuffer, 0 );
	RenderBuffers( context );
}

void CLineManager::RenderBuffers( ID3D11DeviceContext * context )
{
	static UINT Stride = sizeof( SVertex );
	static UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
}

void CLineManager::Shutdown( )
{
	SAFE_RELEASE( m_VertexBuffer );
}

CLineManager::~CLineManager( )
{
}
