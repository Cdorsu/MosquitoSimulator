#pragma once

#include "Shader.h"
#include "LightView.h"
#include "Model.h"

class CSunShadowShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
		DirectX::XMMATRIX World;
	};
	struct SLightVertexBuffer
	{
		DirectX::XMMATRIX LightView;
		DirectX::XMMATRIX LightProjection;
	};
	struct SCameraInfo
	{
		DirectX::XMFLOAT3 CameraPos;
		float pad;
	};
	struct SLightPixelBuffer
	{
		utility::SColor Diffuse;
		utility::SColor Ambient;
		utility::SColor SpecularColor;
		DirectX::XMFLOAT3 LightDir;
		float pad;
	};
	struct SMaterialInfo
	{
		utility::SColor Color;
		BOOL HasTexture;
		BOOL HasSpecularMap;
		BOOL HasNormalMap;
		float SpecularPower;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Buffer;
	ID3D11Buffer * m_MaterialBuffer;
	ID3D11Buffer * m_CameraBuffer;
	ID3D11Buffer * m_LightBufferVS;
	ID3D11Buffer * m_LightBufferPS;
	ID3D11SamplerState * m_WrapSampler;
	ID3D11SamplerState * m_ClampSampler;
public:
	CSunShadowShader( );
	~CSunShadowShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount, DirectX::FXMMATRIX& World,
		CViewInterface * Camera, CModel::SMaterial * MaterialInfo, ID3D11ShaderResourceView * Depthmap,
		CLightView * LightView );
	void SetLightData( ID3D11DeviceContext * context, CLightView * LightView,
		ID3D11ShaderResourceView * Depthmap );
	void SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX& World, CViewInterface * Camera );
	void SetMaterialData( ID3D11DeviceContext * context, CModel::SMaterial * Material );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

