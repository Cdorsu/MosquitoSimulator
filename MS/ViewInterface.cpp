#include "ViewInterface.h"

void CViewInterface::ConstructFrustum( )
{
	using namespace DirectX;
	XMMATRIX ViewProj = GetView( ) * GetProjection( );
	XMFLOAT4X4 ViewProjMat;
	DirectX::XMStoreFloat4x4( &ViewProjMat, ViewProj );
	XMFLOAT4 Plane;
	// Near plane
	Plane.x = ViewProjMat._13;
	Plane.y = ViewProjMat._23;
	Plane.z = ViewProjMat._33;
	Plane.w = ViewProjMat._43;
	m_ViewFrustumPlanes[ 0 ] = Plane;
	// Far plane
	Plane.x = ViewProjMat._14 - ViewProjMat._13;
	Plane.y = ViewProjMat._24 - ViewProjMat._23;
	Plane.z = ViewProjMat._34 - ViewProjMat._33;
	Plane.w = ViewProjMat._44 - ViewProjMat._43;
	m_ViewFrustumPlanes[ 1 ] = Plane;
	// Left plane
	Plane.x = ViewProjMat._14 + ViewProjMat._11;
	Plane.y = ViewProjMat._24 + ViewProjMat._21;
	Plane.z = ViewProjMat._34 + ViewProjMat._31;
	Plane.w = ViewProjMat._44 + ViewProjMat._41;
	m_ViewFrustumPlanes[ 2 ] = Plane;
	// Right plane
	Plane.x = ViewProjMat._14 - ViewProjMat._11;
	Plane.y = ViewProjMat._24 - ViewProjMat._21;
	Plane.z = ViewProjMat._34 - ViewProjMat._31;
	Plane.w = ViewProjMat._44 - ViewProjMat._41;
	m_ViewFrustumPlanes[ 3 ] = Plane;
	// Top plane
	Plane.x = ViewProjMat._14 - ViewProjMat._12;
	Plane.y = ViewProjMat._24 - ViewProjMat._22;
	Plane.z = ViewProjMat._34 - ViewProjMat._32;
	Plane.w = ViewProjMat._44 - ViewProjMat._42;
	m_ViewFrustumPlanes[ 4 ] = Plane;
	// Bottom plane
	Plane.x = ViewProjMat._14 + ViewProjMat._12;
	Plane.y = ViewProjMat._24 + ViewProjMat._22;
	Plane.z = ViewProjMat._34 + ViewProjMat._32;
	Plane.w = ViewProjMat._44 + ViewProjMat._42;
	m_ViewFrustumPlanes[ 5 ] = Plane;

	for ( UINT i = 0; i < m_ViewFrustumPlanes.size( ); ++i )
	{
		float length = sqrt( ( m_ViewFrustumPlanes[ i ].x * m_ViewFrustumPlanes[ i ].x )
			+ ( m_ViewFrustumPlanes[ i ].y * m_ViewFrustumPlanes[ i ].y )
			+ ( m_ViewFrustumPlanes[ i ].z * m_ViewFrustumPlanes[ i ].z ) );
		m_ViewFrustumPlanes[ i ].x /= length;
		m_ViewFrustumPlanes[ i ].y /= length;
		m_ViewFrustumPlanes[ i ].z /= length;
		m_ViewFrustumPlanes[ i ].w /= length;
	}


}

bool CViewInterface::isPointInFrustum( float x, float y, float z )
{
	using namespace DirectX;
	for ( UINT i = 0; i < m_ViewFrustumPlanes.size( ); ++i )
	{
		XMVECTOR planeNormal = XMVectorSet( m_ViewFrustumPlanes[ i ].x, m_ViewFrustumPlanes[ i ].y, m_ViewFrustumPlanes[ i ].z, 0.0f );
		float planeConstant = m_ViewFrustumPlanes[ i ].w;
		XMVECTOR Point = XMVectorSet( x, y, z, 1.0f );
		if ( XMVectorGetX( XMVector3Dot( planeNormal, Point ) ) + planeConstant < 0.0f )
			return false;
	}
	return true;
}

bool CViewInterface::isAABBCompletelyInFrustum( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
{
	using namespace DirectX;
	static std::array<XMVECTOR, 8> Points;
	for ( UINT i = 0; i < m_ViewFrustumPlanes.size( ); ++i )
	{
		XMVECTOR planeNormal = XMVectorSet( m_ViewFrustumPlanes[ i ].x, m_ViewFrustumPlanes[ i ].y, m_ViewFrustumPlanes[ i ].z, 0.0f );
		float planeConstant = m_ViewFrustumPlanes[ i ].w;
		Points[ 0 ] = XMVectorSet( minX, minY, minZ, 1.0f );
		Points[ 1 ] = XMVectorSet( minX, minY, maxZ, 1.0f );
		Points[ 2 ] = XMVectorSet( minX, maxY, minZ, 1.0f );
		Points[ 3 ] = XMVectorSet( maxX, minY, minZ, 1.0f );
		Points[ 4 ] = XMVectorSet( maxX, maxY, minZ, 1.0f );
		Points[ 5 ] = XMVectorSet( maxX, minY, maxZ, 1.0f );
		Points[ 6 ] = XMVectorSet( minX, maxY, maxZ, 1.0f );
		Points[ 7 ] = XMVectorSet( maxX, maxY, maxZ, 1.0f );
		for ( int i = 0; i < Points.size( ); ++i )
			if ( XMVectorGetX( XMVector3Dot( Points[ i ], planeNormal ) ) + planeConstant < 0 )
				return false;
	}
	return true;
}

bool CViewInterface::isAABBPartialInFrustum( float minX, float minY, float minZ, float maxX, float maxY, float maxZ )
{
	if ( isPointInFrustum( minX, minY, minZ ) )
		return true;
	if ( isPointInFrustum( minX, minY, maxZ ) )
		return true;
	if ( isPointInFrustum( minX, maxY, minZ ) )
		return true;
	if ( isPointInFrustum( maxX, minY, minZ ) )
		return true;
	if ( isPointInFrustum( maxX, maxY, minZ ) )
		return true;
	if ( isPointInFrustum( maxX, minY, maxZ ) )
		return true;
	if ( isPointInFrustum( minX, maxY, maxZ ) )
		return true;
	if ( isPointInFrustum( maxX, maxY, maxZ ) )
		return true;
	return false;
}