#include "VerticalBlurShader.h"



CVerticalBlurShader::CVerticalBlurShader( )
{
	ZeroMemory( this, sizeof( CVerticalBlurShader ) );
}

bool CVerticalBlurShader::Initialize( ID3D11Device * device )
{
	HRESULT hr;
	ID3DBlob * VertexShaderBlob;
	if ( !CompileAndCreateVertexShader( device,
		L"VerticalBlurVertexShader.hlsl", L"Shaders\\VerticalBlurVertexShader.cso",
		&m_VertexShader, &VertexShaderBlob ) )
		return false;
	D3D11_INPUT_ELEMENT_DESC layout[ 2 ];
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
	layout[ 1 ].AlignedByteOffset = 0;
	UINT numElements = sizeof( layout ) / sizeof( layout[ 0 ] );
	hr = device->CreateInputLayout( layout, numElements, // Info about the layout
		VertexShaderBlob->GetBufferPointer( ), VertexShaderBlob->GetBufferSize( ), // Info about the vertex shader
		&m_InputLayout ); // The input layout
	IFFAILED( hr, L"Couldn't create a input layout" );
	if ( !CompileAndCreatePixelShader( device,
		L"VerticalBlurPixelShader.hlsl", L"Shaders\\VerticalBlurPixelShader.cso",
		&m_PixelShader ) )
		return false;
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	buffDesc.ByteWidth = sizeof( SPerObject );
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_PerObjectBuffer );
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
	hr = device->CreateSamplerState( &sampDesc, &m_ClampSampler );
	IFFAILED( hr, L"Couldn't create a clamp sampler" );

	return true;
}

void CVerticalBlurShader::SetShaderData( ID3D11DeviceContext * context,
	DirectX::FXMMATRIX& OrtoMatrix,
	ID3D11ShaderResourceView * Texture, float TextureHeight )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	hr = context->Map( m_PerObjectBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	( ( SPerObject* ) MappedResource.pData )->WVP = DirectX::XMMatrixTranspose( OrtoMatrix );
	( ( SPerObject* ) MappedResource.pData )->Height = TextureHeight;
	context->Unmap( m_PerObjectBuffer, 0 );
	context->VSSetConstantBuffers( 0, 1, &m_PerObjectBuffer );
	context->PSSetShaderResources( 0, 1, &Texture );
	context->PSSetSamplers( 0, 1, &m_ClampSampler );
}

void CVerticalBlurShader::SetShaders( ID3D11DeviceContext * context )
{
	context->VSSetShader( m_VertexShader, nullptr, 0 );
	context->PSSetShader( m_PixelShader, nullptr, 0 );
	context->IASetInputLayout( m_InputLayout );
}

void CVerticalBlurShader::Render( ID3D11DeviceContext * context, UINT indexCount,
	DirectX::FXMMATRIX& OrtoMatrix,
	ID3D11ShaderResourceView * Texture, float TextureHeight )
{
	SetShaders( context );
	SetShaderData( context, OrtoMatrix, Texture, TextureHeight );
	DrawIndexed( context, indexCount );
}

void CVerticalBlurShader::Shutdown( )
{
	SAFE_RELEASE( m_VertexShader );
	SAFE_RELEASE( m_PixelShader );
	SAFE_RELEASE( m_PerObjectBuffer );
	SAFE_RELEASE( m_ClampSampler );
	SAFE_RELEASE( m_InputLayout );
}

CVerticalBlurShader::~CVerticalBlurShader( )
{
	Shutdown( );
}
