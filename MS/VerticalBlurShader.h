#pragma once

#include "Shader.h"

class CVerticalBlurShader sealed : public CShader
{
	struct SPerObject
	{
		DirectX::XMMATRIX WVP;
		float Height;
		DirectX::XMFLOAT3 Pad;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11Buffer * m_PerObjectBuffer;
	ID3D11SamplerState * m_ClampSampler;
	ID3D11InputLayout * m_InputLayout;
public:
	CVerticalBlurShader( );
	~CVerticalBlurShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount,
		DirectX::FXMMATRIX& OrtoMatrix,
		ID3D11ShaderResourceView * Texture, float TextureHeight );
	void SetShaderData( ID3D11DeviceContext * context,
		DirectX::FXMMATRIX& OrtoMatrix,
		ID3D11ShaderResourceView * Texture, float TextureHeight );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

