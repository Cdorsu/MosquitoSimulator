#pragma once

#include "common\DirectXIncludes.h"

class CLineManager
{
	struct SVertex
	{
		SVertex( float x = 0.0f, float y = 0.0f, float z = 0.0f )
			:Position( x, y, z ) {};
		DirectX::XMFLOAT3 Position;
	};
private:
	ID3D11Buffer * m_VertexBuffer;
	DirectX::XMFLOAT3 m_3fLastLineFrom;
	DirectX::XMFLOAT3 m_3fLastLineTo;
public:
	CLineManager( );
	~CLineManager( );
private:
	void RenderBuffers( ID3D11DeviceContext * context );
public:
	bool Initialize( ID3D11Device * device );
	void Render( ID3D11DeviceContext * context, DirectX::XMFLOAT3 From, DirectX::XMFLOAT3 To );
	void Shutdown( );
public:
	inline UINT GetVertexCount( ) { return 2; };
};

