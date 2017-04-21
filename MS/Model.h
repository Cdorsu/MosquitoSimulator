#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\DirectXIncludes.h"
#include "Texture.h"

ALIGN16 class CModel sealed
{
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float u = 0.0f, float v = 0.0f )
			: Position( x, y, z ), Texture( u, v ) { };
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};
protected:
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;
	CTexture * m_Texture;
	CTexture * m_Bumpmap;
	CTexture * m_Specularmap;
	UINT m_VertexCount;
	UINT m_IndexCount;
	DirectX::XMMATRIX m_World;
public:
	CModel( );
	~CModel( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context );
	void Shutdown( );
public:
	inline DirectX::XMMATRIX& GetWorld( ) { return m_World; };
	inline ID3D11ShaderResourceView* GetTexture( ) { return m_Texture->GetTexture( ); };
	inline UINT GetIndexCount( ) { return m_IndexCount; };
	inline UINT GetVertexCount( ) { return m_VertexCount; };
public:
	void* operator new ( size_t size );
	void operator delete ( void *object );
};

