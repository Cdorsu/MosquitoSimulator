#include "Skybox.h"



CSkybox::CSkybox( )
{
	ZeroMemory( this, sizeof( CSkybox ) );
}

bool CSkybox::Initialize( ID3D11Device * device, LPWSTR lpTexture )
{
	HRESULT hr;
	ID3D11Texture2D * Texture = nullptr;
	D3DX11_IMAGE_LOAD_INFO imgInfo;
	imgInfo.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
	hr = D3DX11CreateTextureFromFile( device, lpTexture, &imgInfo, nullptr,
		reinterpret_cast< ID3D11Resource** >( &Texture ), nullptr );
	IFFAILED( hr, L"Coldn't load a 3D Texture" );
	D3D11_TEXTURE2D_DESC texDesc;
	Texture->GetDesc( &texDesc );
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
	srvDesc.Format = texDesc.Format;
	srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE;
	hr = device->CreateShaderResourceView( Texture, &srvDesc, &m_Texture );
	IFFAILED( hr, L"Couldn't create shader resource view" );

	SVertex vertices[] =
	{
		SVertex( -1.0f, -1.0f, -1.0f ),
		SVertex( -1.0f, +1.0f, -1.0f ),
		SVertex( +1.0f, +1.0f, -1.0f ),
		SVertex( +1.0f, -1.0f, -1.0f ),
		SVertex( -1.0f, -1.0f, +1.0f ),
		SVertex( -1.0f, +1.0f, +1.0f ),
		SVertex( +1.0f, +1.0f, +1.0f ),
		SVertex( +1.0f, -1.0f, +1.0f ),
	};

	DWORD indices[] = 
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7,
	};

	D3D11_BUFFER_DESC buffDesc = { 0 };
	D3D11_SUBRESOURCE_DATA buffData = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeof( vertices );
	buffData.pSysMem = vertices;
	hr = device->CreateBuffer( &buffDesc, &buffData, &m_VertexBuffer );
	IFFAILED( hr, L"Couldn't create a vertex buffer" );
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeof( indices );
	buffData.pSysMem = indices;
	hr = device->CreateBuffer( &buffDesc, &buffData, &m_IndexBuffer );
	IFFAILED( hr, L"Couldn't create a index buffer" );

	m_World = DirectX::XMMatrixIdentity( );
	m_World *= DirectX::XMMatrixTranslation( 7.0f, 0.0f, 0.0f );

	return true;
}

void CSkybox::Update( CCamera * Camera )
{
	m_World = DirectX::XMMatrixTranslationFromVector( Camera->GetPosition( ) );
}

void CSkybox::Render( ID3D11DeviceContext * context )
{
	static UINT Stride = sizeof( SVertex );
	static UINT Offset = 0;
	context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &Stride, &Offset );
	context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void CSkybox::Shutdown( )
{
	SAFE_RELEASE( m_Texture );
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_IndexBuffer );
}

CSkybox::~CSkybox( )
{
	Shutdown( );
}
