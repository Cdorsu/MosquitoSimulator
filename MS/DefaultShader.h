#pragma once

#include "Shader.h"

class CDefaultShader sealed : public CShader
{
	ID3D11VertexShader * m_VertexShader;
	ID3D11PixelShader * m_PixelShader;
	ID3D11InputLayout * m_InputLayout;
public:
	CDefaultShader( );
	~CDefaultShader( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, UINT indexCount );
	void SetShaders( ID3D11DeviceContext * context );
	void Shutdown( );
};

