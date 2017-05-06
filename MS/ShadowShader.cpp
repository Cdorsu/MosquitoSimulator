#include "ShadowShader.h"



CShadowShader::CShadowShader( )
{
	ZeroMemory( this, sizeof( CShadowShader ) );
}

bool CShadowShader::Initialize( ID3D11Device * device )
{
	HRESULT hr;
	ID3DBlob * VertexShaderBlob;
	if ( !CompileAndCreateVertexShader( device,
		L"ShadowVertexShader.hlsl", L"Shaders\\ShadowVertexShader.cso",
		&m_VertexShader, &VertexShaderBlob ) )
		return false;
	D3D11_INPUT_ELEMENT_DESC layout[ 3 ];
	layout[ 0 ].SemanticName = "POSITION";
	layout[ 0 ].SemanticIndex = 0;
	layout[ 0 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	layout[ 0 ].InputSlot = 0;
	layout[ 0 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 0 ].InstanceDataStepRate = 0;
	layout[ 0 ].AlignedByteOffset = 0;
	layout[ 1 ].SemanticName = "TEXCOORD";
	layout[ 1 ].SemanticIndex = 0;
	layout[ 1 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	layout[ 1 ].InputSlot = 0;
	layout[ 1 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 1 ].InstanceDataStepRate = 0;
	layout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[ 2 ].SemanticName = "NORMAL";
	layout[ 2 ].SemanticIndex = 0;
	layout[ 2 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	layout[ 2 ].InputSlot = 0;
	layout[ 2 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 2 ].InstanceDataStepRate = 0;
	layout[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	UINT numElements = sizeof( layout ) / sizeof( layout[ 0 ] );
	hr = device->CreateInputLayout( layout, numElements, // Info about the layout
		VertexShaderBlob->GetBufferPointer( ), VertexShaderBlob->GetBufferSize( ), // Info about the vertex shader
		&m_InputLayout ); // The input layout
	IFFAILED( hr, L"Couldn't create a input layout" );
	if ( !CompileAndCreatePixelShader( device,
		L"ShadowPixelShader.hlsl", L"Shaders\\ShadowPixelShader.cso",
		&m_PixelShader ) )
		return false;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.ByteWidth = sizeof( SMatrices );
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_Buffer );
	IFFAILED( hr, L"Couldn't create a constant buffer" );
	buffDesc.ByteWidth = sizeof( SLightVertexBuffer );
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_LightBufferVS );
	IFFAILED( hr, L"Couldn't create a constant buffer" );
	buffDesc.ByteWidth = sizeof( SLightPixelBuffer );
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_LightBufferPS );
	IFFAILED( hr, L"Couldn't create a constant buffer" );
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	sampDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MipLODBias = 3;
	hr = device->CreateSamplerState( &sampDesc, &m_WrapSampler );
	IFFAILED( hr, L"Couldn't create a sampler state" );
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = device->CreateSamplerState( &sampDesc, &m_ClampSampler );
	IFFAILED( hr, L"Couldn't create a sampler state" );
	return true;
}

void CShadowShader::Render( ID3D11DeviceContext * context, UINT indexCount, DirectX::FXMMATRIX & World, CViewInterface * Camera, ID3D11ShaderResourceView * Texture, ID3D11ShaderResourceView * Depthmap, CLightView * LightView )
{
	SetShaders( context );
	SetLightData( context, LightView, Depthmap );
	SetData( context, World, Camera );
	SetTextures( context, Texture );
	DrawIndexed( context, indexCount );
}

void CShadowShader::SetLightData( ID3D11DeviceContext * context, CLightView * LightView,
	ID3D11ShaderResourceView * Depthmap )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	hr = context->Map( m_LightBufferVS, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SLightVertexBuffer* ) MappedResource.pData )->LightView = DirectX::XMMatrixTranspose( LightView->GetView( ) );
	( ( SLightVertexBuffer* ) MappedResource.pData )->LightProjection = DirectX::XMMatrixTranspose( LightView->GetProjection( ) );
	( ( SLightVertexBuffer* ) MappedResource.pData )->LightPos = LightView->GetCamPos( );
	context->Unmap( m_LightBufferVS, 0 );
	context->VSSetConstantBuffers( 1, 1, &m_LightBufferVS );
	hr = context->Map( m_LightBufferPS, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SLightPixelBuffer* ) MappedResource.pData )->Diffuse = LightView->GetDiffuse( );
	( ( SLightPixelBuffer* ) MappedResource.pData )->Ambient = LightView->GetAmbient( );
	context->Unmap( m_LightBufferPS, 0 );
	context->PSSetConstantBuffers( 0, 1, &m_LightBufferPS );
	context->PSSetShaderResources( 1, 1, &Depthmap );
	context->PSSetSamplers( 0, 1, &m_WrapSampler );
	context->PSSetSamplers( 1, 1, &m_ClampSampler );
}

void CShadowShader::SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX & World, CViewInterface * Camera )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	static DirectX::XMMATRIX WVP;
	WVP = World * Camera->GetView( ) * Camera->GetProjection( );
	WVP = DirectX::XMMatrixTranspose( WVP );

	hr = context->Map( m_Buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SMatrices* ) MappedResource.pData )->WVP = WVP;
	( ( SMatrices* ) MappedResource.pData )->World = DirectX::XMMatrixTranspose( World );
	context->Unmap( m_Buffer, 0 );
	context->VSSetConstantBuffers( 0, 1, &m_Buffer );
}

void CShadowShader::SetTextures( ID3D11DeviceContext * context,
	ID3D11ShaderResourceView * Texture )
{
	context->PSSetShaderResources( 0, 1, &Texture );
}

void CShadowShader::SetShaders( ID3D11DeviceContext * context )
{
	context->VSSetShader( m_VertexShader, nullptr, 0 );
	context->PSSetShader( m_PixelShader, nullptr, 0 );
	context->IASetInputLayout( m_InputLayout );
}

void CShadowShader::Shutdown( )
{
	SAFE_RELEASE( m_VertexShader );
	SAFE_RELEASE( m_PixelShader );
	SAFE_RELEASE( m_InputLayout );
	SAFE_RELEASE( m_Buffer );
	SAFE_RELEASE( m_LightBufferVS );
	SAFE_RELEASE( m_LightBufferPS );
	SAFE_RELEASE( m_WrapSampler );
}

CShadowShader::~CShadowShader( )
{
}
