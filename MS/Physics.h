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
	static constexpr int WallID = 269;
	static constexpr int MaxXZSpeed2 = 49;
	static constexpr int MaxSpeed2 = 100;
	static constexpr int VerticalImpulse = 5;
	static constexpr float VerticalSpeedToFly2 = 1.0f;
	struct bulletObject
	{
		btRigidBody* Body;
		std::wstring Name;
		unsigned int Score;
		unsigned int Lives = 3;
		bool bTouchesTheGround = false;
		bool bUpdateScoreThisFrame = false;
		bool bUpdateLivesThisFrame = false;
		bulletObject( )
		{
			ZeroMemory( this, sizeof( CPhysics::bulletObject ) );
		};
		bulletObject( std::wstring Name, btRigidBody * Body )
			:Name( Name ), Body( Body )
		{};
	};
	struct SCreateCustomRigidBody
	{
		btScalar mass;
		btCollisionShape * Shape;
		btTransform Trans;
		std::wstring Name;
	};
private:
	btBroadphaseInterface * m_pBroadphase;
	btDispatcher * m_pDispatcher;
	btCollisionConfiguration * m_pCollisionConfiguration;
	btConstraintSolver * m_pSolver;
	btDynamicsWorld * m_pWorld;
	std::vector<bulletObject*> m_vecRigidBodies;
	std::vector<btStridingMeshInterface*> m_vecMeshes;
	std::vector<btCollisionShape*> m_vecCollisionShapes;
protected:
	static std::random_device m_RandomDevice;
	static std::mt19937 m_RandomGenerator;
	static std::uniform_real_distribution<float> m_xzFloatDistribution;
	static std::uniform_real_distribution<float> m_yFloatDistribution;
private: // To be taken from an upper level (CApplication)
	CGraphics * m_Graphics;
	CInput * m_Input;
#if DEBUG || _DEBUG
	void * m_PointerToSomething;
#endif
private: // Deleted automatically
	bulletObject * m_Player;
public:
	CPhysics( );
	~CPhysics( );
public:
	bool Initialize( CGraphics * GraphicsObject, CInput * InputObject );
	btRigidBody* CreateCustomRigidBody/*AndAddItToTheWorld*/( std::vector<CModel::SVertex>& vertices,
		std::vector<DWORD>& indices, btScalar mass, std::wstring Name, btTransform const& Transform );
	btCollisionShape* CreateCustomCollisionShape( std::vector<CModel::SVertex>& vertices,
		std::vector<DWORD>& indices );
	///
	/// <summary>Creates multiple ridig bodies with the input provided</summary>
	/// <param name = "RigidBodiesToCreate">Informations about the rigid bodies to create</param>
	/// <return>Returns a pair of numbers [begin, end) with indices for m_vecRigidBodies (the created bodies)</return>
	///
	std::pair<int,int> CreateMultipleCustomRigidBodies( std::vector<SCreateCustomRigidBody>& RigidBodiesToCreate );
	bool Frame( float fFrameTime );
	void Shutdown( );
public:
	static bool Collision( btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1 );
	static void myTickCallBack( btDynamicsWorld *world, btScalar timeStep );
public:
	static inline bool AABBCollidingWithAABB(
		btVector3& minAABB1, btVector3& maxAABB1,
		btVector3& minAABB2, btVector3& maxAABB2 )
	{
		if ( minAABB2.x( ) < maxAABB1.x( ) && maxAABB2.x( ) > minAABB1.x( ) )
			if ( minAABB2.z( ) < maxAABB1.z( ) && maxAABB2.z( ) > minAABB1.z( ) )
				return true;
		return false;
	}
	static btRigidBody* CreateRigidBody( btScalar Mass, btTransform* trans, btCollisionShape * Shape )
	{
		btDefaultMotionState * MotionState = new btDefaultMotionState( );
		MotionState->setWorldTransform( *trans );
		btRigidBody::btRigidBodyConstructionInfo bodyCI( Mass, MotionState, Shape );
		return new btRigidBody( bodyCI );
	}
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

