#pragma once

#include "Shader.h"

#include "common\utility.h"


class CVerticalBlurShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
		float Height;
		DirectX::XMFLOAT3 Pad;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Buffer;
	ID3D11SamplerState * m_ClampSampler;
public:
	CVerticalBlurShader( );
	~CVerticalBlurShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount,
		DirectX::FXMMATRIX& Projection, ID3D11ShaderResourceView * Texture, float Height );
	void SetData( ID3D11DeviceContext * context,
		DirectX::FXMMATRIX& Projection, ID3D11ShaderResourceView * Texture, float Height );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

