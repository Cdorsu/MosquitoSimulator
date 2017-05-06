#include "RenderTexture.h"



CRenderTexture::CRenderTexture( )
{
	ZeroMemory( this, sizeof( CRenderTexture ) );
}

bool CRenderTexture::Initialize( ID3D11Device * device, UINT WindowWidth, UINT WindowHeight,
	float camNear, float camFar, float FOV, float AspectRatio )
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory( &texDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &srvDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
	ZeroMemory( &rtvDesc, sizeof( D3D11_RENDER_TARGET_VIEW_DESC ) );

	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET |
		D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Width = WindowWidth;
	texDesc.Height = WindowHeight;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	hr = device->CreateTexture2D( &texDesc, nullptr, &m_Texture2D );
	IFFAILED( hr, L"Couldn't create a 2D Texture" );

	srvDesc.Format = texDesc.Format;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
	hr = device->CreateShaderResourceView( m_Texture2D, &srvDesc, &m_ShaderResourceView );
	IFFAILED( hr, L"Couldn't create a shader resource view" );

	rtvDesc.Format = texDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
	hr = device->CreateRenderTargetView( m_Texture2D, &rtvDesc, &m_RenderTargetView );
	IFFAILED( hr, L"Couldn't create a render target view" );


	D3D11_TEXTURE2D_DESC dsviewDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory( &dsviewDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &dsvDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	dsviewDesc.ArraySize = 1;
	dsviewDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	dsviewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsviewDesc.Width = WindowWidth;
	dsviewDesc.Height = WindowHeight;
	dsviewDesc.MipLevels = 1;
	dsviewDesc.SampleDesc.Count = 1;
	dsviewDesc.SampleDesc.Quality = 0;
	dsviewDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	hr = device->CreateTexture2D( &dsviewDesc, nullptr, &m_DSBuffer );
	IFFAILED( hr, L"Couldn't create a DS buffer (texture 2D)" );
	dsvDesc.Format = dsviewDesc.Format;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = device->CreateDepthStencilView( m_DSBuffer, &dsvDesc, &m_DSView );
	IFFAILED( hr, L"Couldn't create a depth stencil view" );

	m_OrthoMatrix = DirectX::
		XMMatrixOrthographicLH( ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight, camNear, camFar );
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH( FOV, AspectRatio, camNear, camFar );

	m_Viewport.Width = ( FLOAT ) WindowWidth;
	m_Viewport.Height = ( FLOAT ) WindowHeight;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftY = 0;
	m_Viewport.TopLeftX = 0;

	return true;
}

void CRenderTexture::Shutdown( )
{
	SAFE_RELEASE( m_Texture2D );
	SAFE_RELEASE( m_DSBuffer );
	SAFE_RELEASE( m_ShaderResourceView );
	SAFE_RELEASE( m_RenderTargetView );
	SAFE_RELEASE( m_DSView );
}

CRenderTexture::~CRenderTexture( )
{
	Shutdown( );
}
