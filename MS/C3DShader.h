#pragma once

#include "Shader.h"
#include "LightView.h"
#include "Model.h"

#include "common\utility.h"


class C3DShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
	};
	struct SColor
	{
		utility::SColor Color;
		BOOL bHasTexture;
		DirectX::XMFLOAT3 Pad;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Matrices;
	ID3D11Buffer * m_Texture;
	ID3D11SamplerState * m_WrapSampler;
public:
	C3DShader( );
	~C3DShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount,
		DirectX::FXMMATRIX& World, CViewInterface * Camera,
		bool bHasTexture, utility::SColor Color,
		ID3D11ShaderResourceView * Texture );
	void SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX& World, CViewInterface* Camera );
	void SetMaterialData( ID3D11DeviceContext * context, bool bHasTexture, utility::SColor Color,
		ID3D11ShaderResourceView * Texture );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

