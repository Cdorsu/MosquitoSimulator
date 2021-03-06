#include "SkyboxShader.h"



CSkyboxShader::CSkyboxShader( )
{
	ZeroMemory( this, sizeof( CSkyboxShader ) );
}

bool CSkyboxShader::Initialize( ID3D11Device * device )
{
	HRESULT hr;
	ID3DBlob * VertexShaderBlob;
	if ( !CompileAndCreateVertexShader( device,
		L"SkyVertexShader.hlsl", L"Shaders\\SkyVertexShader.cso",
		&m_VertexShader, &VertexShaderBlob ) )
		return false;
	D3D11_INPUT_ELEMENT_DESC layout[ 1 ];
	layout[ 0 ].SemanticName = "POSITION";
	layout[ 0 ].SemanticIndex = 0;
	layout[ 0 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	layout[ 0 ].InputSlot = 0;
	layout[ 0 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 0 ].InstanceDataStepRate = 0;
	layout[ 0 ].AlignedByteOffset = 0;
	UINT numElements = sizeof( layout ) / sizeof( layout[ 0 ] );
	hr = device->CreateInputLayout( layout, numElements, // Info about the layout
		VertexShaderBlob->GetBufferPointer( ), VertexShaderBlob->GetBufferSize( ), // Info about the vertex shader
		&m_InputLayout ); // The input layout
	IFFAILED( hr, L"Couldn't create a input layout" );
	if ( !CompileAndCreatePixelShader( device,
		L"SkyPixelShader.hlsl", L"Shaders\\SkyPixelShader.cso",
		&m_PixelShader ) )
		return false;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.ByteWidth = sizeof( SMatrices );
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_Buffer );
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
	return true;
}

void CSkyboxShader::SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX& World,
	CViewInterface * Camera )
{
	static HRESULT hr;
	static DirectX::XMMATRIX WVP;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	WVP = World * Camera->GetView( ) * Camera->GetProjection( );
	WVP = DirectX::XMMatrixTranspose( WVP );
	hr = context->Map( m_Buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SMatrices* ) MappedResource.pData )->WVP = WVP;
	context->Unmap( m_Buffer, 0 );
	context->VSSetConstantBuffers( 0, 1, &m_Buffer );
	context->PSSetSamplers( 0, 1, &m_WrapSampler );
}


void CSkyboxShader::Render( ID3D11DeviceContext * context, UINT indexCount, DirectX::FXMMATRIX& World,
	CViewInterface * Camera, ID3D11ShaderResourceView * Texture )
{
	SetData( context, World, Camera );
	SetTextures( context, Texture );
	SetShaders( context );
	DrawIndexed( context, indexCount );
}

void CSkyboxShader::SetTextures( ID3D11DeviceContext * context, ID3D11ShaderResourceView * Texture )
{
	context->PSSetShaderResources( 0, 1, &Texture );
}

void CSkyboxShader::SetShaders( ID3D11DeviceContext * context )
{
	context->VSSetShader( m_VertexShader, nullptr, 0 );
	context->PSSetShader( m_PixelShader, nullptr, 0 );
	context->IASetInputLayout( m_InputLayout );
}

void CSkyboxShader::Shutdown( )
{
	SAFE_RELEASE( m_VertexShader );
	SAFE_RELEASE( m_PixelShader );
	SAFE_RELEASE( m_InputLayout );
	SAFE_RELEASE( m_Buffer );
	SAFE_RELEASE( m_WrapSampler );
}

CSkyboxShader::~CSkyboxShader( )
{
}
