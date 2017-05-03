#include "Camera.h"



CCamera::CCamera( )
{
	ZeroMemory( this, sizeof( CCamera ) );
}

bool CCamera::InitializeFirstPersonCamera( DirectX::FXMVECTOR& Forward, DirectX::FXMVECTOR& Right,
	DirectX::FXMVECTOR& Position, float AspectRatio, float FOV, float Near, float Far,
	CInput * InputInstance )
{
	m_eCameraType = EType::FirstPersonCamera;
	m_ForwardDirection = Forward;
	m_RightDirection = Right;
	m_UpDirection = DirectX::XMVector3Cross( m_ForwardDirection, m_RightDirection );

	m_Position = Position;
	m_Direction = Forward;
	m_Right = Right;

	m_ViewMatrix = DirectX::XMMatrixLookToLH( m_Position, m_Direction, m_UpDirection );
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH( FOV, AspectRatio, Near, Far );

	m_InputInstance = InputInstance;

	return true;
}

bool CCamera::InitializeThirdPersonCamera( DirectX::FXMVECTOR& Forward, DirectX::FXMVECTOR& Right,
	DirectX::FXMVECTOR& CamTarget, DirectX::CXMVECTOR& CamPosition,
	float DistanceToCam, float AspectRatio, float FOV, float Near, float Far,
	CInput* InputInstance )
{
	m_eCameraType = EType::ThirdPersonCamera;

	m_ForwardDirection = Forward;
	m_RightDirection = Right;
	m_UpDirection = DirectX::XMVector3Cross( m_ForwardDirection, m_RightDirection );

	m_Direction = CamTarget;
	m_Position = CamPosition;
	
	m_ViewMatrix = DirectX::XMMatrixLookAtLH( CamPosition, CamTarget, m_UpDirection );
	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH( FOV, AspectRatio, Near, Far );

	m_fDistanceToCamera = DistanceToCam;

	m_InputInstance = InputInstance;

	return true;
}

void CCamera::Update( )
{
	static DirectX::XMMATRIX RotationMatrix;
	m_fCamYaw += m_InputInstance->GetHorizontalMouseMove( ) * 0.001f;
	m_fCamPitch += m_InputInstance->GetVerticalMouseMove( ) * 0.001f;
	if ( m_fCamPitch > 1.5f )
		m_fCamPitch = 1.5f;
	else if ( m_fCamPitch < -1.5f )
		m_fCamPitch = -1.5f;
	/*if ( m_InputInstance->isKeyPressed( DIK_W ) )
		m_fMoveForwardBackward += 5 * 0.02f;
	if ( m_InputInstance->isKeyPressed( DIK_S ) )
		m_fMoveForwardBackward -= 5 * 0.02f;
	if ( m_InputInstance->isKeyPressed( DIK_D ) )
		m_fMoveRightLeft += 5 * 0.02f;
	if ( m_InputInstance->isKeyPressed( DIK_A ) )
		m_fMoveRightLeft -= 5 * 0.02f;*/

	if ( m_eCameraType == EType::FirstPersonCamera )
	{

		RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw( m_fCamPitch, m_fCamYaw, m_fCamRoll );
		m_Direction = DirectX::XMVector3TransformNormal( m_ForwardDirection, RotationMatrix );
		m_Right = DirectX::XMVector3TransformNormal( m_RightDirection, RotationMatrix );
		m_UpDirection = DirectX::XMVector3Cross( m_Direction, m_Right );

		m_Position = DirectX::XMVectorAdd( m_Position, DirectX::XMVectorScale( m_Direction, m_fMoveForwardBackward ) );
		m_Position = DirectX::XMVectorAdd( m_Position, DirectX::XMVectorScale( m_Right, m_fMoveRightLeft ) );

		m_fMoveForwardBackward = 0;
		m_fMoveRightLeft = 0;
		m_ViewMatrix = DirectX::XMMatrixLookToLH( m_Position, m_Direction, m_UpDirection );
	}
	else
	{
		float mouseWheelMove = m_InputInstance->GetMouseWheelMove( );
		if ( mouseWheelMove )
		{
			if ( mouseWheelMove > 0.0f )
			{
				m_fDistanceToCamera -= 1.0f;
				if ( m_fDistanceToCamera < 3.0f )
					m_fDistanceToCamera = 3;
			}
			else
			{
				m_fDistanceToCamera += 1.0f;
				if ( m_fDistanceToCamera > 20.0f )
					m_fDistanceToCamera = 20.0f;
			}
		}
		RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw( m_fCamPitch, m_fCamYaw, m_fCamRoll );
		m_Position = DirectX::XMVector3TransformCoord(
			DirectX::XMVectorSubtract( DirectX::XMVectorZero( ),
				m_ForwardDirection ), RotationMatrix );
		m_Position = DirectX::XMVector3Normalize( m_Position );

		m_Position = DirectX::XMVectorScale( m_Position, m_fDistanceToCamera );
		m_Position = DirectX::XMVectorAdd( m_Direction, m_Position );

		m_ViewMatrix = DirectX::XMMatrixLookAtLH( m_Position, m_Direction, m_UpDirection );

		m_fMoveForwardBackward = 0;
		m_fMoveRightLeft = 0;
	}
}

void CCamera::Render( )
{
}

void CCamera::Shutdown( )
{

}

CCamera::~CCamera( )
{
	Shutdown( );
}
