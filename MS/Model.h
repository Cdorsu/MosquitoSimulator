#pragma once

#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\DirectXIncludes.h"

class CModel sealed
{
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f )
			: Position( x, y, z ) { };
		DirectX::XMFLOAT3 Position;
	};
protected:
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;
	UINT m_VertexCount;
	UINT m_IndexCount;
public:
	CModel( );
	~CModel( );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context );
	void Shutdown( );
public:
	inline UINT GetIndexCount( ) { return m_IndexCount; };
	inline UINT GetVertexCount( ) { return m_VertexCount; };
};

