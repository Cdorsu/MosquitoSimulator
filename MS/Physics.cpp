#include "Physics.h"

btMatrix3x3 CPhysics::m_3x3RotationMatrix = btMatrix3x3( );
std::random_device CPhysics::m_RandomDevice;
std::mt19937 CPhysics::m_RandomGenerator = std::mt19937( CPhysics::m_RandomDevice( ) );
std::uniform_real_distribution<float> CPhysics::m_xzFloatDistribution = std::uniform_real_distribution<float>( -50, 50 );
std::uniform_real_distribution<float> CPhysics::m_yFloatDistribution = std::uniform_real_distribution<float>( 0, 30 );

CPhysics::CPhysics( )
{
	m_3x3RotationMatrix.setEulerYPR( 0, 0, SIMD_HALF_PI );
}

bool CPhysics::Initialize( CGraphics * GraphicsObject, CInput * InputObject )
{
	m_Graphics = GraphicsObject;
	m_Input = InputObject;

	m_Input->addSpecialKey( DIK_SPACE );

	m_pBroadphase = new btDbvtBroadphase( );
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration( );
	m_pDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );
	m_pSolver = new btSequentialImpulseConstraintSolver( );
	m_pWorld = new btDiscreteDynamicsWorld( m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration );

	btCollisionShape *PlaneShape = new btBoxShape( btVector3( 50, 0, 50 ) );
	btMotionState *MotionState = new btDefaultMotionState( );
	btVector3 localInertia = btVector3( 0, 0, 0 );
	btRigidBody::btRigidBodyConstructionInfo PlaneCI( 0, MotionState, PlaneShape, localInertia );
	btRigidBody *Plane = new btRigidBody( PlaneCI );
	Plane->setFlags( Plane->getFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	Plane->setUserIndex( GroundID );
	Plane->setFriction( 3 );
	bulletObject *PlanePtr = new bulletObject( L"Plane", Plane );
	Plane->setUserPointer( PlanePtr );
	m_pWorld->addRigidBody( Plane );
	m_vecRigidBodies.push_back( PlanePtr );

	btCollisionShape *BoxShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	btMotionState *BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 10, 0 ) ) );
	btRigidBody::btRigidBodyConstructionInfo BoxCI( 50, BoxState, BoxShape );
	btRigidBody *Box = new btRigidBody( BoxCI );
	
	bulletObject *BoxPtr = new bulletObject( L"Box", Box );
	Box->setUserPointer( BoxPtr );
	m_pWorld->addRigidBody( Box );
	m_vecRigidBodies.push_back( BoxPtr );

	BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 30, 0 ) ) );
	localInertia = btVector3( 10, 0, 0 );
	BoxCI = btRigidBody::btRigidBodyConstructionInfo( 50, BoxState, BoxShape, localInertia );
	Box = new btRigidBody( BoxCI );
	BoxPtr = new bulletObject( L"Box", Box );
	Box->setUserPointer( BoxPtr );
	m_pWorld->addRigidBody( Box );
	m_vecRigidBodies.push_back( BoxPtr );

	auto TorusMaxAABB = m_Graphics->GetTorus( )->GetMaxAABB( );
	float x, y, z;
	x = m_xzFloatDistribution( m_RandomGenerator );
	z = m_xzFloatDistribution( m_RandomGenerator );
	y = m_yFloatDistribution( m_RandomGenerator );
	btCollisionShape * TorusShape = new btBoxShape( btVector3( TorusMaxAABB.x, TorusMaxAABB.y, TorusMaxAABB.z ) );
	btMotionState * TorusState = new btDefaultMotionState( btTransform( m_3x3RotationMatrix, btVector3( x, y, z ) ) );
	btRigidBody::btRigidBodyConstructionInfo TorusCI( 0, TorusState, TorusShape, localInertia );
	btRigidBody * Torus = new btRigidBody( TorusCI );
	Torus->setCollisionFlags( Torus->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_NO_CONTACT_RESPONSE |
		btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	bulletObject * TorusPtr = new bulletObject( L"Torus", Torus );
	Torus->setUserPointer( TorusPtr );
	Torus->setUserIndex( CheckpointID );
	Torus->setGravity( btVector3( 0, 0, 0 ) );
	m_pWorld->addRigidBody( Torus );
	m_vecRigidBodies.push_back( TorusPtr );

#if !(_DEBUG || DEBUG)
	DirectX::XMFLOAT3 MaxAABB = m_Graphics->GetPlayerAABB( ).second;
#else
	DirectX::XMFLOAT3 MaxAABB = DirectX::XMFLOAT3( 3.f, 3.f, 3.f );
#endif
	btCollisionShape * CapsuleShape = new btBoxShape( btVector3( MaxAABB.x, MaxAABB.y - 0.7f, MaxAABB.z ) );
	btMotionState * CapsuleState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 10, 15, 10 ) ) );
	btRigidBody::btRigidBodyConstructionInfo CapsuleCI( 100, CapsuleState, CapsuleShape );
	btRigidBody * Capsule = new btRigidBody( CapsuleCI );
	Capsule->setFlags( Capsule->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	Capsule->setActivationState( DISABLE_DEACTIVATION );
	bulletObject * CapsulePtr = new bulletObject( L"Player", Capsule );
	CapsulePtr->Score = 0;
	Capsule->setUserPointer( CapsulePtr );
	Capsule->setUserIndex( PlayerID );
	m_pWorld->addRigidBody( Capsule );
	m_vecRigidBodies.push_back( CapsulePtr );

	m_Player = CapsulePtr;

	gContactAddedCallback = Collision;
	
	m_pWorld->setGravity( btVector3( 0, -10, 0 ) );

#if DEBUG || _DEBUG
	this->setDebugMode( CPhysics::DebugDrawModes::DBG_DrawAabb );
	m_pWorld->setDebugDrawer( this );
#endif
	m_pWorld->setInternalTickCallback( CPhysics::myTickCallBack, reinterpret_cast< void* >( this ) );

	return true;
}

void CPhysics::Frame( float fFrameTime )
{
	m_Player->bTouchesTheGround = false;
	m_pWorld->stepSimulation( fFrameTime );
	for ( unsigned int i = 0; i < m_vecRigidBodies.size( ); ++i )
	{
		if ( m_vecRigidBodies[ i ]->Name == L"Plane" )
		{
			float matrix[ 16 ];
			btMotionState* motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderPlane( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Box" )
		{
			float matrix[ 16 ];
			btMotionState* motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderCube( matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Torus" )
		{
			float matrix[ 16 ];
			btMotionState* motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderTorus( matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Player" )
		{
			float matrix[ 16 ];
			btMotionState * motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			m_Graphics->RenderPlayer( DirectX::XMFLOAT3(
				trans.getOrigin( ).x( ),
				trans.getOrigin( ).y( ),
				trans.getOrigin( ).z( ) ), matrix );
		}
	}
	if ( m_Input->isKeyPressed( DIK_W ) )
	{
		CCamera * Camera = m_Graphics->GetCamera( );
		auto CamForward = Camera->GetDirection( );
		btVector3 Direction = btVector3( CamForward.x, 0.0f, CamForward.z );
		btVector3 InitialVelocity = m_Player->Body->getLinearVelocity( );
		btVector3 xzVelocity = btVector3( InitialVelocity.x( ), 0, InitialVelocity.z( ) );
		if ( xzVelocity.length2( ) < MaxXZSpeed2 )
			InitialVelocity += Direction.normalize( );
		m_Player->Body->setLinearVelocity( InitialVelocity );
		btTransform btTrans = m_Player->Body->getCenterOfMassTransform( );
		btMatrix3x3 Rotation;
		float rad = atan2f( InitialVelocity.x( ), InitialVelocity.z( ) );
		Rotation.setEulerYPR( 0, SIMD_PI + rad, 0 );
		m_Player->Body->setCenterOfMassTransform( btTransform( Rotation, btTrans.getOrigin( ) ) );
	}
	else if ( m_Input->isKeyPressed( DIK_S ) )
	{
		CCamera * Camera = m_Graphics->GetCamera( );
		auto CamForward = Camera->GetDirection( );
		btVector3 Direction = btVector3( CamForward.x, 0.0f, CamForward.z );
		btVector3 InitialVelocity = m_Player->Body->getLinearVelocity( );
		btVector3 xzVelocity = btVector3( InitialVelocity.x( ), 0, InitialVelocity.z( ) );
		if ( xzVelocity.length2( ) < MaxXZSpeed2 )
			InitialVelocity -= Direction.normalize( );
		m_Player->Body->setLinearVelocity( InitialVelocity );
		btTransform btTrans = m_Player->Body->getCenterOfMassTransform( );
		btMatrix3x3 Rotation;
		float rad = atan2f( InitialVelocity.x( ), InitialVelocity.z( ) );
		Rotation.setEulerYPR( 0, SIMD_PI + rad, 0 );
		m_Player->Body->setCenterOfMassTransform( btTransform( Rotation, btTrans.getOrigin( ) ) );
	}
	if ( m_Input->isKeyPressed( DIK_D ) )
	{
		CCamera * Camera = m_Graphics->GetCamera( );
		auto CamForward = Camera->GetRight( );
		btVector3 Direction = btVector3( CamForward.x, 0.0f, CamForward.z );
		btVector3 InitialVelocity = m_Player->Body->getLinearVelocity( );
		btVector3 xzVelocity = btVector3( InitialVelocity.x( ), 0, InitialVelocity.z( ) );
		if ( xzVelocity.length2( ) < MaxXZSpeed2 )
			InitialVelocity += Direction.normalize( );
		m_Player->Body->setLinearVelocity( InitialVelocity );
		btTransform btTrans = m_Player->Body->getCenterOfMassTransform( );
		btMatrix3x3 Rotation;
		float rad = atan2f( InitialVelocity.x( ), InitialVelocity.z( ) );
		Rotation.setEulerYPR( 0, SIMD_PI + rad, 0 );
		m_Player->Body->setCenterOfMassTransform( btTransform( Rotation, btTrans.getOrigin( ) ) );
	}
	else if ( m_Input->isKeyPressed( DIK_A ) )
	{
		CCamera * Camera = m_Graphics->GetCamera( );
		auto CamForward = Camera->GetRight( );
		btVector3 Direction = btVector3( CamForward.x, 0.0f, CamForward.z );
		btVector3 InitialVelocity = m_Player->Body->getLinearVelocity( );
		btVector3 xzVelocity = btVector3( InitialVelocity.x( ), 0, InitialVelocity.z( ) );
		if ( xzVelocity.length2( ) < MaxXZSpeed2 )
			InitialVelocity -= Direction.normalize( );
		m_Player->Body->setLinearVelocity( InitialVelocity );
		btTransform btTrans = m_Player->Body->getCenterOfMassTransform( );
		btMatrix3x3 Rotation;
		float rad = atan2f( InitialVelocity.x( ), InitialVelocity.z( ) );
		Rotation.setEulerYPR( 0, SIMD_PI + rad, 0 );
		m_Player->Body->setCenterOfMassTransform( btTransform( Rotation, btTrans.getOrigin( ) ) );
	}
	else if ( m_Input->isSpecialKeyPressed( DIK_SPACE ) )
	{
		CCamera * Camera = m_Graphics->GetCamera( );
		auto CamForward = Camera->GetDirection( );
		btVector3 Direction = btVector3( CamForward.x, 0.0f, CamForward.z );
		btVector3 InitialVelocity = m_Player->Body->getLinearVelocity( );
		Direction.normalize( );
		InitialVelocity += Direction + btVector3( 0, btScalar( VerticalImpulse ), 0 );
		m_Player->Body->setLinearVelocity( InitialVelocity );
		btTransform btTrans = m_Player->Body->getCenterOfMassTransform( );
		btMatrix3x3 Rotation;
		InitialVelocity.setY( 0 );
		InitialVelocity.normalize( );
		float rad = atan2f( InitialVelocity.x( ), InitialVelocity.z( ) );
		Rotation.setEulerYPR( 0, SIMD_PI + rad, 0 );
		m_Player->Body->setCenterOfMassTransform( btTransform( Rotation, btTrans.getOrigin( ) ) );
		
	}
	if ( m_Input->isKeyPressed( DIK_B ) )
	{
		m_pWorld->debugDrawWorld( );
	}
	m_Graphics->SetUserTouchesTheGround( m_Player->bTouchesTheGround );
	m_Graphics->SetScore( m_Player->Score );
	m_Player->bUpdateScoreThisFrame = false;
}

void CPhysics::Shutdown( )
{
	for ( unsigned int i = 0; i < m_vecRigidBodies.size( ); ++i )
	{
		m_pWorld->removeCollisionObject( m_vecRigidBodies[ i ]->Body );
		delete m_vecRigidBodies[ i ]->Body->getCollisionShape( );
		delete m_vecRigidBodies[ i ]->Body->getMotionState( );
		delete m_vecRigidBodies[ i ]->Body;
		delete m_vecRigidBodies[ i ];
	}

	delete m_pWorld;

	delete m_pSolver;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
	delete m_pBroadphase;
}

CPhysics::~CPhysics( )
{
}

bool CPhysics::Collision( btManifoldPoint& cp,
	const btCollisionObjectWrapper* obj0, int partId0, int index0,
	const btCollisionObjectWrapper* obj1, int partId1, int index1 ) // Will be called twice for every collision
{
	bulletObject * First = ( ( bulletObject* ) obj0->getCollisionObject( )->getUserPointer( ) );
	bulletObject * Second = ( ( bulletObject* ) obj1->getCollisionObject( )->getUserPointer( ) );
	if ( First == nullptr || Second == nullptr )
		return false;
	btRigidBody * Torus = nullptr;
	btRigidBody * User = nullptr;
	if ( First->Body->getUserIndex( ) == CheckpointID )
		Torus = First->Body;
	else if ( Second->Body->getUserIndex( ) == CheckpointID )
		Torus = Second->Body;
	if ( First->Body->getUserIndex( ) == PlayerID )
		User = First->Body;
	else if ( Second->Body->getUserIndex( ) == PlayerID )
		User = Second->Body;
	if ( User == nullptr )
		return false;
	if ( Torus == nullptr )
		return false;
	float x;
	float z;
	float y;
	bool Done = false;
	while ( !Done ) // Make sure we don't place the checkpoint inside player's AABB
	{
		btVector3 UserMinAABB, UserMaxAABB;
		btVector3 TorusMinAABB, TorusMaxAABB;
		x = CPhysics::m_xzFloatDistribution( CPhysics::m_RandomGenerator );
		z = CPhysics::m_xzFloatDistribution( CPhysics::m_RandomGenerator );
		y = CPhysics::m_yFloatDistribution( CPhysics::m_RandomGenerator );
		Torus->setWorldTransform( btTransform( m_3x3RotationMatrix, btVector3( x, y, z ) ) );
		Torus->getMotionState( )->setWorldTransform( btTransform( m_3x3RotationMatrix, btVector3( x, y, z ) ) );
		User->getAabb( UserMinAABB,UserMaxAABB );
		Torus->getAabb( TorusMinAABB, TorusMaxAABB );
		if ( !AABBCollidingWithAABB( UserMinAABB, UserMaxAABB, TorusMinAABB, TorusMaxAABB ) )
			Done = true;
	}
	Torus->setLinearVelocity( btVector3( 0, 0, 0 ) );
	Torus->setGravity( btVector3( 0, 0, 0 ) );
	Torus->activate( );
	if ( !( ( bulletObject* ) User->getUserPointer( ) )->bUpdateScoreThisFrame )
	{
		( ( bulletObject* ) User->getUserPointer( ) )->Score = ( ( bulletObject* ) User->getUserPointer( ) )->Score + 1;
		( ( bulletObject* ) User->getUserPointer( ) )->bUpdateScoreThisFrame = true;
	}

	return true;
}

void CPhysics::myTickCallBack( btDynamicsWorld *world, btScalar timeStep )
{
	CPhysics* object = ( CPhysics* ) world->getWorldUserInfo( );
	for ( unsigned int i = 0; i < object->m_vecRigidBodies.size( ); ++i )
	{
		btVector3 Velocity = object->m_vecRigidBodies[ i ]->Body->getLinearVelocity( );
		btScalar Speed = Velocity.length2( );
		if ( Speed > MaxSpeed2 )
		{
			Velocity *= MaxSpeed2 / Speed;
			object->m_vecRigidBodies[ i ]->Body->setLinearVelocity( Velocity );
		}
	}
}