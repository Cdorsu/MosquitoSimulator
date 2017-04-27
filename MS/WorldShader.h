#pragma once

#include "Shader.h"

class CWorldShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
		DirectX::XMMATRIX World;
		DirectX::XMFLOAT3 CamPos;
		float pad;
	};
	struct SLight
	{
		DirectX::XMFLOAT3 Direction;
		float SpecPower;
		utility::SColor Diffuse;
		utility::SColor Ambient;
		utility::SColor SpecColor;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Buffer;
	ID3D11Buffer * m_LightBuffer;
	ID3D11SamplerState * m_WrapSampler;
public:
	CWorldShader( );
	~CWorldShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount, DirectX::FXMMATRIX& World,
		CCamera * Camera, ID3D11ShaderResourceView * Texture, ID3D11ShaderResourceView * Specular,
		CLight * Light );
	void SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX& World, CCamera * Camera );
	void SetLightData( ID3D11DeviceContext * context, CLight * Light );
	void SetShaders( ID3D11DeviceContext * context );
	void SetTextures( ID3D11DeviceContext * context, ID3D11ShaderResourceView * Texture,
		ID3D11ShaderResourceView * Specular );
	void Shutdown( );
};

