#pragma once

#include "Shader.h"

class CSkyboxShader sealed : public CShader
{
	struct SMatrices
	{
		DirectX::XMMATRIX WVP;
	};
private:
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
	ID3D11Buffer * m_Buffer;
	ID3D11SamplerState * m_WrapSampler;
public:
	CSkyboxShader( );
	~CSkyboxShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount, DirectX::FXMMATRIX& World,
		CViewInterface * Camera, ID3D11ShaderResourceView * Texture );
	void SetData( ID3D11DeviceContext * context, DirectX::FXMMATRIX& World, CViewInterface * Camera );
	void SetShaders( ID3D11DeviceContext * context );
	void SetTextures( ID3D11DeviceContext * context, ID3D11ShaderResourceView * Texture );
	void Shutdown( );
};

