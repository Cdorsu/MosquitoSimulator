#pragma once

#include "Texture.h"
#include "common\windowsIncludes.h"
#include "common\DirectXIncludes.h"

class CTextureWindow sealed
{
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float u = 0.0f, float v = 0.0f )
			:Pos( x, y, z ), Tex( u, v ) { };
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};
private:
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;
	CTexture * m_Texture;

	float m_fWindowWidth;
	float m_fWindowHeight;
	float m_fImageWidth;
	float m_fImageHeight;
	float m_fPreviousX;
	float m_fPreviousY;
public:
	CTextureWindow( );
	~CTextureWindow( );
public:
	bool Initialize( ID3D11Device * device, LPWSTR lpTexturePath,
		UINT WindowWidth, UINT WindowHeight, UINT ImageWidth, UINT ImageHeight );
	void Render( ID3D11DeviceContext * context, UINT PosX, UINT PosY );
	void Shutdown( );
private:
	void SetShaders( ID3D11DeviceContext * context );
public:
	inline UINT GetIndexCount( ) { return 6; }; // Might be static, but I don't want it to be static
	inline UINT GetVertexCount( ) { return 4; }; // Might be static, but I don't want it to be static
	inline ID3D11ShaderResourceView* GetTexture( ) { return m_Texture->GetTexture( ); }; // I want it to be static, but it can't be static:(
};

