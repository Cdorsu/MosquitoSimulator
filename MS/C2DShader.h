#pragma once

#include "Shader.h"

#include "common\utility.h"


class C2DShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
	};
	struct SColor
	{
		utility::SColor Color;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Buffer;
	ID3D11Buffer * m_ColorBuffer;
	ID3D11SamplerState * m_WrapSampler;
public:
	C2DShader( );
	~C2DShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount,
		DirectX::FXMMATRIX& Projection, ID3D11ShaderResourceView * Texture,
		utility::SColor Color = utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	void SetData( ID3D11DeviceContext * context,
		DirectX::FXMMATRIX& Projection, ID3D11ShaderResourceView * Texture,
		utility::SColor Color = utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

