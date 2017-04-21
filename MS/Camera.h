#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include "Input.h"

__declspec( align( 16 ) ) class CCamera
{
	DirectX::XMVECTOR m_ForwardDirection;
	DirectX::XMVECTOR m_RightDirection;
	DirectX::XMVECTOR m_UpDirection;

	DirectX::XMMATRIX m_ViewMatrix;
	DirectX::XMMATRIX m_ProjectionMatrix;

	DirectX::XMVECTOR m_Position;
	DirectX::XMVECTOR m_Direction;
	DirectX::XMVECTOR m_Right;

	float m_fCamPitch;
	float m_fCamYaw;
	float m_fCamRoll;
	float m_fMoveForwardBackward;
	float m_fMoveRightLeft;

	CInput* m_InputInstance;
public:
	CCamera( );
	~CCamera( );
public:
	bool Initialize( DirectX::FXMVECTOR& Forward, DirectX::FXMVECTOR& Right,
		DirectX::FXMVECTOR& Position, float AspectRatio, float FOV, float Near, float Far,
		CInput* InputInstance );
	void Update( );
	void Render( );
	void Shutdown( );
public:
	inline void SetPosition( DirectX::FXMVECTOR& Position ) { m_Position = Position; };
	inline DirectX::XMMATRIX& GetView( ) { return m_ViewMatrix; };
	inline DirectX::XMMATRIX& GetProjection( ) { return m_ProjectionMatrix; };
public:
	inline void* operator new ( size_t size )
	{
		return _aligned_malloc( size,16 );
	};
	inline void operator delete ( void * object )
	{
		_aligned_free( object );
	};
};

