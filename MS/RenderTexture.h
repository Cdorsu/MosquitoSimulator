#pragma once

#include "common\definitions.h"
#include "common\utility.h"
#include "common\DirectXIncludes.h"


ALIGN16 class CRenderTexture
{
	ID3D11Texture2D * m_Texture2D;
	ID3D11Texture2D * m_DSBuffer;
	ID3D11ShaderResourceView * m_ShaderResourceView;
	ID3D11RenderTargetView * m_RenderTargetView;
	ID3D11DepthStencilView * m_DSView;

	DirectX::XMMATRIX m_OrthoMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;
public:
	CRenderTexture( );
	~CRenderTexture( );
public:
	bool Initialize( ID3D11Device * device, UINT WindowWidth, UINT WindowHeight,
		float camNear, float camFar, float FOV, float AspectRatio );
	void Shutdown( );
public:
	inline void SetRenderTarget( ID3D11DeviceContext * context )
	{
		ID3D11RenderTargetView * RTVS = nullptr;
		ID3D11ShaderResourceView * SRVS = nullptr;
		context->OMSetRenderTargets( 1, &RTVS, nullptr );
		context->PSSetShaderResources( 0, 1, &SRVS );
		context->OMSetRenderTargets( 1, &m_RenderTargetView, m_DSView );
	}
	inline void BeginScene( ID3D11DeviceContext * context, utility::SColor Color )
	{
		context->ClearRenderTargetView( m_RenderTargetView, Color );
		context->ClearDepthStencilView( m_DSView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH |
			D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0 );
	}
	inline ID3D11ShaderResourceView* GetTexture( )
	{
		return m_ShaderResourceView;
	}
	inline DirectX::XMMATRIX& GetOrthoMatrix( ) { return m_OrthoMatrix; };
	inline DirectX::XMMATRIX& GetProjectionMatrix( ) { return m_ProjectionMatrix; };
public:
	inline void* operator new ( size_t size )
	{
		return _aligned_malloc( size, 16 );
	}
	inline void operator delete ( void* object )
	{
		_aligned_free( object );
	}
};

