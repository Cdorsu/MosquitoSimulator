#pragma once
#include "ViewInterface.h"
#include "common\utility.h"
ALIGN16 class CLightView sealed :
	public CViewInterface
{
	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;
	
	DirectX::XMVECTOR m_Position;
	DirectX::XMVECTOR m_LookAt;
	DirectX::XMVECTOR m_UpDirection = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	utility::SColor m_DiffuseColor;
	utility::SColor m_AmbientColor;
public:
	CLightView( ) { };
	~CLightView( ) { };
public:
	inline void SetLookAt( DirectX::FXMVECTOR& LookAt ) { m_LookAt = LookAt; };
	inline void SetPosition( DirectX::FXMVECTOR& Position ) { m_Position = Position; };
	inline void SetDiffuse( utility::SColor newDiffuse ) { m_DiffuseColor = newDiffuse; };
	inline void SetAmbient( utility::SColor newAmbient ) { m_AmbientColor = newAmbient; };
	inline void GenerateProjectionMatrix( float FOV, float AspectRatio, float CamNear, float CamFar )
	{
		m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH( FOV, AspectRatio, CamNear, CamFar );
	}
	inline void GenerateViewMatrix( )
	{
		m_ViewMatrix = DirectX::XMMatrixLookAtLH( m_Position, m_LookAt, m_UpDirection );
	}
public:
	inline utility::SColor GetDiffuse( ) { return m_DiffuseColor; };
	inline utility::SColor GetAmbient( ) { return m_AmbientColor; };
public:
	virtual inline DirectX::XMMATRIX& GetView( ) override { return m_ViewMatrix; };
	virtual inline DirectX::XMMATRIX& GetProjection( ) override { return m_ProjectionMatrix; };
	virtual inline DirectX::XMFLOAT3 GetCamPos( ) override
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMStoreFloat3( &Pos, m_Position );
		return Pos;
	}
public:
	inline void* operator new ( size_t size )
	{
		return _aligned_malloc( size, 16 );
	}
	inline void operator delete ( void* object )
	{
		return _aligned_free( object );
	}
};

