#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\DirectXIncludes.h"
#include "common\utility.h"

ALIGN16 class CD3D11 sealed
{
	UINT m_SampleCount;
	UINT m_SampleQuality;
public:
	ID3D11Device * m_d3d11Device;
	ID3D11DeviceContext * m_d3d11DeviceContext;
	ID3D11RenderTargetView * m_d3d11RenderTargetView;
	ID3D11DepthStencilView * m_d3d11DSView;
	IDXGISwapChain * m_SwapChain;
	D3D11_VIEWPORT m_DefaultViewport;

	ID3D11RasterizerState * m_NoCulling;
	ID3D11DepthStencilState * m_DSLessEqual;

	utility::SColor m_BackgroundColor;
	LPWSTR m_GPUInfo;
	UINT m_DedicatedVideoMemory;
	DirectX::XMMATRIX m_OrthoMatrix;
public:
	CD3D11( );
	~CD3D11( );
public:
	bool Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, float Near, float Far, bool bFullscreen );
	void Shutdown( );
public:
	void BeginScene( );
	void EndScene( );
public:
	inline ID3D11Device * GetDevice( ) { return m_d3d11Device; };
	inline ID3D11DeviceContext * GetImmediateContext( ) { return m_d3d11DeviceContext; };
	inline void DisableCulling( ) { m_d3d11DeviceContext->RSSetState( m_NoCulling ); };
	inline void EnableBackFaceCulling( ) { m_d3d11DeviceContext->RSSetState( nullptr ); };
	inline void EnableDefaultDSState( ) { m_d3d11DeviceContext->OMSetDepthStencilState( nullptr, 0 ); };
	inline void EnableDSLessEqual( ) { m_d3d11DeviceContext->OMSetDepthStencilState( m_DSLessEqual, 0 ); };
	inline void EnableBackBuffer( ) { m_d3d11DeviceContext->OMSetRenderTargets( 1, &m_d3d11RenderTargetView, m_d3d11DSView ); };
	inline DirectX::XMMATRIX& GetOrthoMatrix( ) { return m_OrthoMatrix; };
public:
	void * operator new ( size_t size );
	void operator delete ( void* object );
};

