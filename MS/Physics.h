#pragma once

#include "Graphics.h"
#include "Input.h"
#pragma warning ( push )
#pragma warning ( disable : 4305 )
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#pragma warning ( pop )


class CPhysics sealed : public btIDebugDraw
{
	static constexpr int CheckpointID = 69;
	static constexpr int PlayerID = 169;
	static constexpr int MaxXZSpeed2 = 25;
	static constexpr int VerticalImpulse = 5;
	struct bulletObject
	{
		btRigidBody* Body;
		std::wstring Name;
		unsigned int Score;
		bulletObject( )
		{
			ZeroMemory( this, sizeof( CPhysics::bulletObject ) );
		};
		bulletObject( std::wstring Name, btRigidBody * Body )
			:Name( Name ), Body( Body )
		{};
	};
private:
	btBroadphaseInterface * m_pBroadphase;
	btDispatcher * m_pDispatcher;
	btCollisionConfiguration * m_pCollisionConfiguration;
	btConstraintSolver * m_pSolver;
	btDynamicsWorld * m_pWorld;
	std::vector<bulletObject*> m_vecRigidBodies;
protected:
	static std::random_device m_RandomDevice;
	static std::mt19937 m_RandomGenerator;
	static std::uniform_real_distribution<float> m_xzFloatDistribution;
	static std::uniform_real_distribution<float> m_yFloatDistribution;
private: // To be taken from an upper level (CApplication)
	CGraphics * m_Graphics;
	CInput * m_Input;
private: // Deleted automatically
	bulletObject * m_Player;
public:
	CPhysics( );
	~CPhysics( );
public:
	bool Initialize( CGraphics * GraphicsObject, CInput * InputObject );
	void Frame( float fFrameTime );
	void Shutdown( );
public:
	static bool Collision( btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1 );
protected:
	static btMatrix3x3 m_3x3RotationMatrix;
public: // Inherited
	void drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
	{
		float fX = from.x( ), fY = from.y( ), fZ = from.z( );
		float tX = to.x( ), tY = to.z( ), tZ = to.z( );
		utility::SColor Color( color.x( ), color.y( ), color.z( ), color.w( ) );
		m_Graphics->RenderLine( DirectX::XMFLOAT3( fX, fY, fZ ), DirectX::XMFLOAT3( tX, tY, tZ ), Color );
	}
	void drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
	{
		OutputDebugString( L"ContactPoint?\n" );
	}
	void reportErrorWarning( const char* arg )
	{
		OutputDebugStringA( arg );
	}
	void draw3dText( const btVector3&, const char* )
	{
		OutputDebugString( L"3DTEXT\n" );
	}
	void setDebugMode( int DebugMode )
	{
		m_iDebugMode = DebugMode;
	}
	int getDebugMode( ) const
	{
		return m_iDebugMode;
	}
private:
	int m_iDebugMode;

};

