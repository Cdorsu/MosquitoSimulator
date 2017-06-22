#pragma once

#include "LightView.h"

class CSunLightView sealed :
	public CLightView
{
	DirectX::XMVECTOR m_Direction;
public:
	inline void SetDirection( DirectX::FXMVECTOR& Direction ) { m_Direction = Direction; };
	virtual inline DirectX::XMFLOAT3 GetDirection( ) override
	{
		DirectX::XMFLOAT3 Direction;
		DirectX::XMStoreFloat3( &Direction, m_Direction );
		return Direction;
	};
	inline void GenerateProjectionMatrix( float WidthHeight, float CamNear, float CamFar )
	{
		m_ProjectionMatrix = DirectX::XMMatrixOrthographicLH( WidthHeight, WidthHeight, CamNear, CamFar );
	}
	inline void GenerateViewMatrix( ) override
	{
		m_ViewMatrix = DirectX::XMMatrixLookToLH( m_Position, m_Direction, m_UpDirection );
	}
};