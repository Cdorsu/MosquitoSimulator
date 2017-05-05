#pragma once

#include "common\definitions.h"
#include "common\DirectXIncludes.h"
#include "Camera.h"

ALIGN16 class CSkybox sealed
{
private:
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f )
			: Pos( x, y, z ) { };
		DirectX::XMFLOAT3 Pos;
	};
private:
	ID3D11ShaderResourceView * m_Texture;
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;
	DirectX::XMMATRIX m_World;
public:
	CSkybox( );
	~CSkybox( );
public:
	bool Initialize( ID3D11Device * device, LPWSTR lpTexturePath );
	void Update( CCamera * Camera );
	void Render( ID3D11DeviceContext * context );
	void Shutdown( );
public:
	inline DirectX::XMMATRIX& GetWorld( ) { return m_World; };
	inline ID3D11ShaderResourceView* GetTexture( ) { return m_Texture; };
public:
	inline void* operator new ( size_t size )
	{
		return _aligned_malloc( size,16 );
	};
	inline void operator delete ( void* object )
	{
		_aligned_free( object );
	};
};

