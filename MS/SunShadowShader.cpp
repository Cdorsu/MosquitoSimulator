#include "SunShadowShader.h"



CSunShadowShader::CSunShadowShader( )
{
	ZeroMemory( this, sizeof( CSunShadowShader ) );
}

bool CSunShadowShader::Initialize( ID3D11Device * device )
{
	HRESULT hr;
	ID3DBlob * VertexShaderBlob;
	if ( !CompileAndCreateVertexShader( device,
		L"SunVertexShader.hlsl", L"Shaders\\SunVertexShader.cso",
		&m_VertexShader, &VertexShaderBlob ) )
		return false;
	D3D11_INPUT_ELEMENT_DESC layout[ 5 ];
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
	layout[ 3 ].SemanticName = "TANGENT";
	layout[ 3 ].SemanticIndex = 0;
	layout[ 3 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	layout[ 3 ].InputSlot = 0;
	layout[ 3 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 3 ].InstanceDataStepRate = 0;
	layout[ 3 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[ 4 ].SemanticName = "BINORMAL";
	layout[ 4 ].SemanticIndex = 0;
	layout[ 4 ].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	layout[ 4 ].InputSlot = 0;
	layout[ 4 ].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	layout[ 4 ].InstanceDataStepRate = 0;
	layout[ 4 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	UINT numElements = sizeof( layout ) / sizeof( layout[ 0 ] );
	hr = device->CreateInputLayout( layout, numElements, // Info about the layout
		VertexShaderBlob->GetBufferPointer( ), VertexShaderBlob->GetBufferSize( ), // Info about the vertex shader
		&m_InputLayout ); // The input layout
	IFFAILED( hr, L"Couldn't create a input layout" );
	if ( !CompileAndCreatePixelShader( device,
		L"SunPixelShader.hlsl", L"Shaders\\SunPixelShader.cso",
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
	buffDesc.ByteWidth = sizeof( SCameraInfo );
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_CameraBuffer );
	IFFAILED( hr, L"Couldn't create a constatn buffer" );
	buffDesc.ByteWidth = sizeof( SMaterialInfo );
	hr = device->CreateBuffer( &buffDesc, nullptr, &m_MaterialBuffer );
	IFFAILED( hr, L"Couldn't create a constatn buffer" );
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

void CSunShadowShader::Render( ID3D11DeviceContext * context, UINT indexCount,
	DirectX::FXMMATRIX & World, CViewInterface * Camera,
	CModel::SMaterial * MaterialInfo, ID3D11ShaderResourceView * Depthmap, CLightView * LightView )
{
	SetShaders( context );
	SetLightData( context, LightView, Depthmap );
	SetMaterialData( context, MaterialInfo );
	SetData( context, World, Camera );
	DrawIndexed( context, indexCount );
}

void CSunShadowShader::SetMaterialData( ID3D11DeviceContext * context, CModel::SMaterial * MaterialInfo )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	hr = context->Map( m_MaterialBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SMaterialInfo* ) MappedResource.pData )->HasTexture = MaterialInfo->bHasTexture;
	( ( SMaterialInfo* ) MappedResource.pData )->HasSpecularMap = MaterialInfo->Specularmap ? 1 : 0;
	( ( SMaterialInfo* ) MappedResource.pData )->HasNormalMap = MaterialInfo->Bumpmap ? 1 : 0;
	( ( SMaterialInfo* ) MappedResource.pData )->Color = MaterialInfo->DiffuseColor;
	( ( SMaterialInfo* ) MappedResource.pData )->SpecularPower = MaterialInfo->SpecularPower;
	context->Unmap( m_MaterialBuffer, 0 );
	context->PSSetConstantBuffers( 1, 1, &m_MaterialBuffer );
	if ( MaterialInfo->Texture != nullptr )
	{
		ID3D11ShaderResourceView * SRV = ( MaterialInfo->Texture->GetTexture( ) );
		context->PSSetShaderResources( 0, 1, &SRV );
		if ( MaterialInfo->Specularmap != nullptr )
		{
			SRV = MaterialInfo->Specularmap->GetTexture( );
			context->PSSetShaderResources( 2, 1, &SRV );
		}
		if ( MaterialInfo->Bumpmap != nullptr )
		{
			SRV = MaterialInfo->Bumpmap->GetTexture( );
			context->PSSetShaderResources( 3, 1, &SRV );
		}
	}
}

void CSunShadowShader::SetLightData( ID3D11DeviceContext * context, CLightView * LightView,
	ID3D11ShaderResourceView * Depthmap )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;

	hr = context->Map( m_LightBufferVS, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SLightVertexBuffer* ) MappedResource.pData )->LightView = DirectX::XMMatrixTranspose( LightView->GetView( ) );
	( ( SLightVertexBuffer* ) MappedResource.pData )->LightProjection = DirectX::XMMatrixTranspose( LightView->GetProjection( ) );
	context->Unmap( m_LightBufferVS, 0 );
	context->VSSetConstantBuffers( 1, 1, &m_LightBufferVS );
	hr = context->Map( m_LightBufferPS, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SLightPixelBuffer* ) MappedResource.pData )->Diffuse = LightView->GetDiffuse( );
	( ( SLightPixelBuffer* ) MappedResource.pData )->Ambient = LightView->GetAmbient( );
	( ( SLightPixelBuffer* ) MappedResource.pData )->SpecularColor = LightView->GetSpecular( );
	( ( SLightPixelBuffer* ) MappedResource.pData )->LightDir = LightView->GetDirection( );
	context->Unmap( m_LightBufferPS, 0 );
	context->PSSetConstantBuffers( 0, 1, &m_LightBufferPS );
	context->PSSetShaderResources( 1, 1, &Depthmap );
	context->PSSetSamplers( 0, 1, &m_WrapSampler );
	context->PSSetSamplers( 1, 1, &m_ClampSampler );
}

void CSunShadowShader::SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX & World, CViewInterface * Camera )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	static DirectX::XMMATRIX WVP;
#ifndef _USE_TRANSPOSED_WORLD_MATRIX_
	WVP = World * Camera->GetView( ) * Camera->GetProjection( );
#else
	WVP = DirectX::XMMatrixTranspose( World ) * Camera->GetView( ) * Camera->GetProjection( );
#endif // _USE_TRANSPOSED_WORLD_MATRIX

	WVP = DirectX::XMMatrixTranspose( WVP );

	hr = context->Map( m_Buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SMatrices* ) MappedResource.pData )->WVP = WVP;
#ifndef _USE_TRANSPOSED_WORLD_MATRIX_
	( ( SMatrices* ) MappedResource.pData )->World = DirectX::XMMatrixTranspose( World );
#else
	( ( SMatrices* ) MappedResource.pData )->World = World;
#endif // _USE_TRANSPOSED_WORLD_MATRIX_
	context->Unmap( m_Buffer, 0 );
	context->VSSetConstantBuffers( 0, 1, &m_Buffer );
	hr = context->Map( m_CameraBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	if ( FAILED( hr ) )
		return;
	( ( SCameraInfo* ) MappedResource.pData )->CameraPos = Camera->GetCamPos( );
	context->Unmap( m_CameraBuffer, 0 );
	context->VSSetConstantBuffers( 2, 1, &m_CameraBuffer );
}

void CSunShadowShader::SetShaders( ID3D11DeviceContext * context )
{
	context->VSSetShader( m_VertexShader, nullptr, 0 );
	context->PSSetShader( m_PixelShader, nullptr, 0 );
	context->IASetInputLayout( m_InputLayout );
}

void CSunShadowShader::Shutdown( )
{
	SAFE_RELEASE( m_VertexShader );
	SAFE_RELEASE( m_PixelShader );
	SAFE_RELEASE( m_InputLayout );
	SAFE_RELEASE( m_Buffer );
	SAFE_RELEASE( m_MaterialBuffer );
	SAFE_RELEASE( m_CameraBuffer );
	SAFE_RELEASE( m_LightBufferVS );
	SAFE_RELEASE( m_LightBufferPS );
	SAFE_RELEASE( m_WrapSampler );
}

CSunShadowShader::~CSunShadowShader( )
{
}
