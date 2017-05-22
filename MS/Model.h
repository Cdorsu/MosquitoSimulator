#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\DirectXIncludes.h"
#include "common\utility.h"
#include "Texture.h"

ALIGN16 class CMosquito sealed;

ALIGN16 class CModel sealed
{
	friend class CMosquito;
public:
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float u = 0.0f, float v = 0.0f,
			float nx = 0.0f, float ny = 0.0f, float nz = 0.0f )
			: Position( x, y, z ), Texture( u, v ), Normal( nx, ny, nz )
		{
			ZeroMemory( this, sizeof( SVertex ) );
		};
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
		DirectX::XMFLOAT3 Binormal;
	};
	struct SMaterial
	{
		CTexture * Texture;
		CTexture * Bumpmap;
		CTexture * Specularmap;
		utility::SColor DiffuseColor;
		utility::SColor SpecularColor;
		float SpecularPower;
		bool bHasTexture = false;
	};
private:
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;
	SMaterial * m_Material;

public:
	CModel( );
	~CModel( );
public:
	bool Initialize( ID3D11Device * device );
	bool Initialize( ID3D11Device * device, LPWSTR lpFile );
	void Render( ID3D11DeviceContext * context );
	void Shutdown( );
public:
	static bool ReadFile( ID3D11Device * device, LPWSTR lpFilepath,
		UINT& VertexCount, UINT& IndexCount,
		std::vector<SVertex>& Vertices, std::vector<DWORD>& Indices,
		SMaterial* Material, UINT toAddToIndices = 0 );
protected:
	std::vector<SVertex> m_vecVertices;
	std::vector<DWORD> m_vecIndices;
	UINT m_VertexCount;
	UINT m_IndexCount;
	DirectX::XMMATRIX m_World;
public:
	inline DirectX::XMMATRIX& GetWorld( ) { return m_World; };
	inline ID3D11ShaderResourceView* GetTexture( ) { if ( m_Material->Texture != nullptr ) return m_Material->Texture->GetTexture( ); else return nullptr; };
	inline ID3D11ShaderResourceView* GetSpecularMap( ) { return m_Material->Specularmap->GetTexture( ); };
	inline ID3D11ShaderResourceView* GetBumpmap( ) { return m_Material->Bumpmap->GetTexture( ); };
	inline SMaterial* GetMaterial( ) { return m_Material; };
	inline UINT GetIndexCount( ) { return m_IndexCount; };
	inline UINT GetVertexCount( ) { return m_VertexCount; };
	inline float GetSpecularPower( ) { return m_Material->SpecularPower; };
	inline utility::SColor GetSpecularColor( ) { return m_Material->SpecularColor; };
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

