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
			float u = 0.0f, float v = 0.0f,
			float nx = 0.0f, float ny = 0.0f, float nz = 0.0f )
			: Position( x, y, z ), Texture( u, v ), Normal( nx, ny, nz ) { };
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
		DirectX::XMFLOAT3 Normal;
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

	std::vector<SVertex> m_vecVertices;
	std::vector<DWORD> m_vecIndices;
public:
	CModel( );
	~CModel( );
public:
	bool Initialize( ID3D11Device * device );
	bool Initialize( ID3D11Device * device, LPWSTR lpFile );
	void Render( ID3D11DeviceContext * context );
	void Shutdown( );
public:
	inline DirectX::XMMATRIX& GetWorld( ) { return m_World; };
	inline ID3D11ShaderResourceView* GetTexture( ) { return m_Texture->GetTexture( ); };
	inline UINT GetIndexCount( ) { return m_IndexCount; };
	inline UINT GetVertexCount( ) { return m_VertexCount; };
public:
	inline void Identity( ) { m_World = DirectX::XMMatrixIdentity( ); };
	inline void Translate( float x, float y, float z ) { m_World *= DirectX::XMMatrixTranslation( x, y, z ); };
	inline void Scale( float x, float y, float z ) { m_World *= DirectX::XMMatrixScaling( x, y, z ); };
	inline void RotateX( float theta ) { m_World *= DirectX::XMMatrixRotationX( theta ); };
	inline void RotateY( float theta ) { m_World *= DirectX::XMMatrixRotationY( theta ); };
	inline void RotateZ( float theta ) { m_World *= DirectX::XMMatrixRotationZ( theta ); };
public:
	void* operator new ( size_t size );
	void operator delete ( void *object );
};

