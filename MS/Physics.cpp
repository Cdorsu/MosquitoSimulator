#include "Physics.h"

btMatrix3x3 CPhysics::m_3x3RotationMatrix = btMatrix3x3( );
std::random_device CPhysics::m_RandomDevice;
std::mt19937 CPhysics::m_RandomGenerator = std::mt19937( CPhysics::m_RandomDevice( ) );
std::uniform_real_distribution<float> CPhysics::m_xzFloatDistribution = std::uniform_real_distribution<float>( -45, 45 );
std::uniform_real_distribution<float> CPhysics::m_yFloatDistribution = std::uniform_real_distribution<float>( 10, 45 );

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
	m_vecCollisionShapes.push_back( PlaneShape );
	btMotionState *MotionState = new btDefaultMotionState( );
	btRigidBody::btRigidBodyConstructionInfo PlaneCI( 0, MotionState, PlaneShape );
	btRigidBody *Plane = new btRigidBody( PlaneCI );
	Plane->setFriction( 3 );
	bulletObject *PlanePtr = new bulletObject( L"Plane", Plane );
	Plane->setUserPointer( PlanePtr );
	m_pWorld->addRigidBody( Plane );
	m_vecRigidBodies.push_back( PlanePtr );

	btMatrix3x3 RotMat;
	PlaneShape = new btBoxShape( btVector3( 50, 0, 50 ) );
	m_vecCollisionShapes.push_back( PlaneShape );
	MotionState = new btDefaultMotionState( );
	MotionState->setWorldTransform( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 50, 0 ) ) );
	PlaneCI = btRigidBody::btRigidBodyConstructionInfo( 0, MotionState, PlaneShape );
	Plane = new btRigidBody( PlaneCI );
	Plane->setRestitution( 1.0f );
	Plane->setUserIndex( WallID );
	Plane->setCollisionFlags( Plane->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	PlanePtr = new bulletObject( L"Ceiling", Plane );
	Plane->setUserPointer( PlanePtr );
	m_pWorld->addRigidBody( Plane );
	m_vecRigidBodies.push_back( PlanePtr );
	
	btCollisionShape * WallShape = new btBoxShape( btVector3( 50, 50, 0 ) );
	m_vecCollisionShapes.push_back( WallShape );
	btMotionState * WallState = new btDefaultMotionState( );
	btVector3 localInertia = btVector3( 0, 0, 0 );
	WallState->setWorldTransform( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 0, -50 ) ) );
	btRigidBody::btRigidBodyConstructionInfo WallCI( 0, WallState, WallShape );
	btRigidBody * Wall = new btRigidBody( WallCI );
	Wall->setRestitution( 1.0f );
	Wall->setUserIndex( WallID );
	Wall->setCollisionFlags( Wall->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	bulletObject *WallPtr = new bulletObject( L"Wall", Wall );
	Wall->setUserPointer( WallPtr );
	m_pWorld->addRigidBody( Wall );
	m_vecRigidBodies.push_back( WallPtr );

	RotMat.setEulerYPR( 0, SIMD_HALF_PI, 0 );
	WallShape = new btBoxShape( btVector3( 50, 50, 0 ) );
	m_vecCollisionShapes.push_back( WallShape );
	WallState = new btDefaultMotionState( );
	WallState->setWorldTransform( btTransform( RotMat, btVector3( -50, 0, 0 ) ) );
	WallCI = btRigidBody::btRigidBodyConstructionInfo( 0, WallState, WallShape );
	Wall = new btRigidBody( WallCI );
	Wall->setRestitution( 1.0f );
	Wall->setUserIndex( WallID );
	Wall->setCollisionFlags( Wall->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	WallPtr = new bulletObject( L"Wall", Wall );
	Wall->setUserPointer( WallPtr );
	m_pWorld->addRigidBody( Wall );
	m_vecRigidBodies.push_back( WallPtr );

	RotMat.setEulerYPR( 0, SIMD_PI, 0 );
	WallShape = new btBoxShape( btVector3( 50, 50, 0 ) );
	m_vecCollisionShapes.push_back( WallShape );
	WallState = new btDefaultMotionState( );
	WallState->setWorldTransform( btTransform( RotMat, btVector3( 0, 0, 50 ) ) );
	WallCI = btRigidBody::btRigidBodyConstructionInfo( 0, WallState, WallShape );
	Wall = new btRigidBody( WallCI );
	Wall->setRestitution( 1.0f );
	Wall->setUserIndex( WallID );
	Wall->setCollisionFlags( Wall->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	WallPtr = new bulletObject( L"Wall", Wall );
	Wall->setUserPointer( WallPtr );
	m_pWorld->addRigidBody( Wall );
	m_vecRigidBodies.push_back( WallPtr );

	RotMat.setEulerYPR( 0, SIMD_PI + SIMD_HALF_PI, 0 );
	WallShape = new btBoxShape( btVector3( 50, 50, 0 ) );
	m_vecCollisionShapes.push_back( WallShape );
	WallState = new btDefaultMotionState( );
	WallState->setWorldTransform( btTransform( RotMat, btVector3( 50, 0, 0 ) ) );
	WallCI = btRigidBody::btRigidBodyConstructionInfo( 0, WallState, WallShape );
	Wall = new btRigidBody( WallCI );
	Wall->setRestitution( 1.0f );
	Wall->setCollisionFlags( Wall->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	Wall->setUserIndex( WallID );
	WallPtr = new bulletObject( L"Wall", Wall );
	Wall->setUserPointer( WallPtr );
	m_pWorld->addRigidBody( Wall );
	m_vecRigidBodies.push_back( WallPtr );

	auto TableVertices = m_Graphics->GetTable( )->GetVertices( );
	auto TableIndices = m_Graphics->GetTable( )->GetIndices( );
	btRigidBody * Table = CreateCustomRigidBody( TableVertices, TableIndices, 0, L"Table",
		btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 4, 0 ) ) );
	Table->getCollisionShape( )->setLocalScaling( btVector3( 7, 7, 7 ) );

	auto DoggoVertices = m_Graphics->GetDoggo( )->GetVertices( );
	auto DoggoIndices = m_Graphics->GetDoggo( )->GetIndices( );
	btRigidBody * Doggo = CreateCustomRigidBody( DoggoVertices, DoggoIndices, 250, L"Doggo",
		btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 20, 6, 30 ) ) );

	auto ChairVertices = m_Graphics->GetChair( )->GetVertices( );
	auto ChairIndices = m_Graphics->GetChair( )->GetIndices( );
	btCollisionShape * ChairShape = CreateCustomCollisionShape( ChairVertices, ChairIndices );
	m_vecCollisionShapes.push_back( ChairShape );
	ChairShape->setLocalScaling( btVector3( 5, 5, 5 ) );
	std::vector<SCreateCustomRigidBody> BodiesToCreate;
	SCreateCustomRigidBody Body;
	Body.mass = 200;
	Body.Name = L"Chair";
	Body.Shape = ChairShape;
	Body.Trans = btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 7, 20 ) );
	BodiesToCreate.push_back( Body );
	Body.mass = 200;
	Body.Name = L"Chair";
	Body.Shape = ChairShape;
	RotMat.setEulerYPR( 0, SIMD_PI, 0 );
	Body.Trans = btTransform( RotMat, btVector3( 0, 7, -20 ) );
	BodiesToCreate.push_back( Body );
	Body.mass = 200;
	Body.Name = L"Chair";
	Body.Shape = ChairShape;
	RotMat.setEulerYPR( 0, SIMD_PI / 2, 0 );
	Body.Trans = btTransform( RotMat, btVector3( 20, 7, 0 ) );
	BodiesToCreate.push_back( Body );
	Body.mass = 200;
	Body.Name = L"Chair";
	Body.Shape = ChairShape;
	RotMat.setEulerYPR( 0, -SIMD_PI / 2, 0 );
	Body.Trans = btTransform( RotMat, btVector3( -20, 7, 0 ) );
	BodiesToCreate.push_back( Body );
	CreateMultipleCustomRigidBodies( BodiesToCreate );
	

	btCollisionShape *BoxShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	m_vecCollisionShapes.push_back( BoxShape );
	btMotionState *BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 30, 1, 0 ) ) );
	btRigidBody::btRigidBodyConstructionInfo BoxCI( 5, BoxState, BoxShape );
	btRigidBody *Box = new btRigidBody( BoxCI );
	
	bulletObject *BoxPtr = new bulletObject( L"Box", Box );
	Box->setUserPointer( BoxPtr );
	m_pWorld->addRigidBody( Box );
	m_vecRigidBodies.push_back( BoxPtr );

	m_pWorld->getSolverInfo( ).m_numIterations = 10;

	btScalar SpringLength( 3 );
	btScalar PivotOffset( 0 );
	btScalar SpringRange( 1 );

	PlaneShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	m_vecCollisionShapes.push_back( PlaneShape );
	btTransform tr;
	tr.setIdentity( );
	tr.setOrigin( btVector3( 0, 0, 0 ) );
	tr.getBasis( ).setEulerYPR( 0, 0, 0 );
	MotionState = new btDefaultMotionState( );
	MotionState->setWorldTransform( tr );
	btRigidBody* pBodyA = new btRigidBody( 1, MotionState, PlaneShape );
	m_pWorld->addRigidBody( pBodyA );
	bulletObject * objPtr = new bulletObject( L"Lightbulb", pBodyA );
	pBodyA->setUserPointer( objPtr );
	m_vecRigidBodies.push_back( objPtr );

	btTransform frameInA;
	frameInA = btTransform::getIdentity( );
	frameInA.setOrigin( btVector3( 0, 0, 0 ) );

	tr.setIdentity( );
	tr.setOrigin( btVector3( 0, 60 - SpringLength - PivotOffset, 0 ) );
	tr.getBasis( ).setEulerYPR( 0, 0, 0 );

	MotionState = new btDefaultMotionState( );
	MotionState->setWorldTransform( tr );
	BoxShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	m_vecCollisionShapes.push_back( BoxShape );
	btRigidBody* pBodyB = new btRigidBody( 0, MotionState, BoxShape );
	m_pWorld->addRigidBody( pBodyB );
	objPtr = new bulletObject( L"Don't draw", pBodyB );
	pBodyB->setUserPointer( objPtr );
	m_vecRigidBodies.push_back( objPtr );

	btTransform frameInB;
	frameInB = btTransform::getIdentity( );
	frameInB.setOrigin( btVector3( PivotOffset, -10, 0 ) );

	btGeneric6DofSpringConstraint * pGen6DOFSpring = new btGeneric6DofSpringConstraint(
		*pBodyA, *pBodyB, frameInA, frameInB, true
	);

	pGen6DOFSpring->setLinearLowerLimit( btVector3(  - SpringRange,  - 0,  - SpringRange ) );
	pGen6DOFSpring->setLinearUpperLimit( btVector3(  + SpringRange,  + 0,  + SpringRange ) );

	pGen6DOFSpring->setAngularLowerLimit( btVector3( - SpringRange, - SpringRange, - SpringRange ) );
	pGen6DOFSpring->setAngularUpperLimit( btVector3( + SpringRange, + SpringRange, + SpringRange ) );

	m_pWorld->addConstraint( pGen6DOFSpring, true );

	for ( int i = 0; i < 3; ++i )
	{
		pGen6DOFSpring->enableSpring( i, true );
		//pGen6DOFSpring->enableSpring( i + 3, true );
		pGen6DOFSpring->setStiffness( i, 1 );
		pGen6DOFSpring->setStiffness( i + 3, 1 );
		pGen6DOFSpring->setDamping( i, btScalar( 0.5 ) );
		pGen6DOFSpring->setDamping( i + 3, btScalar( 0.4 ) );
		pGen6DOFSpring->setParam( BT_CONSTRAINT_STOP_CFM, btScalar( (1.0E-5) ), i );
		pGen6DOFSpring->setParam( BT_CONSTRAINT_STOP_CFM, btScalar( ( 1.0E-5 ) ), i + 3 );
		pGen6DOFSpring->setParam( BT_CONSTRAINT_CFM, btScalar( ( 1.0E-5 ) ), i );
		pGen6DOFSpring->setParam( BT_CONSTRAINT_CFM, btScalar( ( 1.0E-5 ) ), i+3 );
	}
	pGen6DOFSpring->setEquilibriumPoint( );


	BoxShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	m_vecCollisionShapes.push_back( BoxShape );
	BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 3, -10 ) ) );
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
	m_vecCollisionShapes.push_back( TorusShape );
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
	m_vecCollisionShapes.push_back( CapsuleShape );
	btMotionState * CapsuleState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 10, 15, 10 ) ) );
	btRigidBody::btRigidBodyConstructionInfo CapsuleCI( 50, CapsuleState, CapsuleShape );
	btRigidBody * Capsule = new btRigidBody( CapsuleCI );
	Capsule->setFlags( Capsule->getCollisionFlags( ) | btRigidBody::CollisionFlags::CF_CUSTOM_MATERIAL_CALLBACK );
	Capsule->setActivationState( DISABLE_DEACTIVATION );
	Capsule->setRestitution( 1.0f );
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

bool CPhysics::Frame( float fFrameTime )
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
			m_Graphics->RenderGround( matrix );
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
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderPlayer( DirectX::XMFLOAT3(
				trans.getOrigin( ).x( ),
				trans.getOrigin( ).y( ),
				trans.getOrigin( ).z( ) ), matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Wall" )
		{
			float matrix[ 16 ];
			btMotionState * motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderWall( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Ceiling" )
		{
			float matrix[ 16 ];
			btMotionState * motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderCeiling( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Lightbulb" )
		{
			float matrix[ 16 ];
			btMotionState * motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderLightBulb( matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Table" )
		{
			float matrix[ 16 ], world[ 16 ], scale[ 16 ];
			ZeroMemory( scale, sizeof( float ) * 16 );
			btRigidBody * Body = m_vecRigidBodies[ i ]->Body;
			btCollisionShape * Shape = Body->getCollisionShape( );
			btVector3 scaling = Shape->getLocalScaling( );
			btMotionState * motionState = Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( world );
			scale[ utility::toIndex( 0, 0 ) ] = scaling.x( );
			scale[ utility::toIndex( 1, 1 ) ] = scaling.y( );
			scale[ utility::toIndex( 2, 2 ) ] = scaling.z( );
			scale[ utility::toIndex( 3, 3 ) ] = 1;
			utility::MatrixMultiply( scale, world, matrix );
			m_Graphics->RenderTable( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Chair" )
		{
			float matrix[ 16 ], world[ 16 ], scale[ 16 ];
			ZeroMemory( scale, sizeof( float ) * 16 );
			btRigidBody * Body = m_vecRigidBodies[ i ]->Body;
			btCollisionShape * Shape = Body->getCollisionShape( );
			btVector3 scaling = Shape->getLocalScaling( );
			btMotionState * motionState = Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( world );
			scale[ utility::toIndex( 0, 0 ) ] = scaling.x( );
			scale[ utility::toIndex( 1, 1 ) ] = scaling.y( );
			scale[ utility::toIndex( 2, 2 ) ] = scaling.z( );
			scale[ utility::toIndex( 3, 3 ) ] = 1;
			utility::MatrixMultiply( scale, world, matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderChair( matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Doggo" )
		{
			float matrix[ 16 ], world[ 16 ], scale[ 16 ];
			ZeroMemory( scale, sizeof( float ) * 16 );
			btRigidBody * Body = m_vecRigidBodies[ i ]->Body;
			btCollisionShape * Shape = Body->getCollisionShape( );
			btVector3 scaling = Shape->getLocalScaling( );
			btMotionState * motionState = Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( world );
			scale[ utility::toIndex( 0, 0 ) ] = scaling.x( );
			scale[ utility::toIndex( 1, 1 ) ] = scaling.y( );
			scale[ utility::toIndex( 2, 2 ) ] = scaling.z( );
			scale[ utility::toIndex( 3, 3 ) ] = 1;
			utility::MatrixMultiply( scale, world, matrix );
			btVector3 minAABB, maxAABB;
			m_vecRigidBodies[ i ]->Body->getAabb( minAABB, maxAABB );
			m_Graphics->RenderDoggo( matrix,
				minAABB.x( ), minAABB.y( ), minAABB.z( ),
				maxAABB.x( ), maxAABB.y( ), maxAABB.z( ) );
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
#if DEBUG || _DEBUG
		m_pWorld->debugDrawWorld( );
#endif
	}
	m_Graphics->SetUserTouchesTheGround( m_Player->bTouchesTheGround );
	m_Graphics->SetScore( m_Player->Score );
	m_Player->bUpdateScoreThisFrame = false;
	m_Player->bUpdateLivesThisFrame = false;
	return m_Graphics->SetLives( m_Player->Lives );
}

btRigidBody* CPhysics::CreateCustomRigidBody/*AndAddItToTheWorld*/( std::vector<CModel::SVertex>& vertices, std::vector<DWORD>& indices,
	btScalar mass, std::wstring Name, btTransform const& Trans)
{
	btTriangleMesh * TriangleMesh = new btTriangleMesh( true, false );
	m_vecMeshes.push_back( TriangleMesh );
	for ( unsigned int i = 0; i < indices.size( ) / 3; ++i )
	{
		CModel::SVertex* Vertex1 = &vertices[ indices[ i * 3 + 0 ] ];
		CModel::SVertex* Vertex2 = &vertices[ indices[ i * 3 + 1 ] ];
		CModel::SVertex* Vertex3 = &vertices[ indices[ i * 3 + 2 ] ];
		TriangleMesh->addTriangle(
			btVector3( Vertex1->Position.x, Vertex1->Position.y, Vertex1->Position.z ),
			btVector3( Vertex2->Position.x, Vertex2->Position.y, Vertex2->Position.z ),
			btVector3( Vertex3->Position.x, Vertex3->Position.y, Vertex3->Position.z ), true
		);
	}
	btBvhTriangleMeshShape * TriangleShape = new btBvhTriangleMeshShape( TriangleMesh, true );
	m_vecCollisionShapes.push_back( TriangleShape );
	btMotionState * TriangleState = new btDefaultMotionState(
		Trans
	);
	
	btRigidBody::btRigidBodyConstructionInfo TriangleCI( mass, TriangleState, TriangleShape );
	btRigidBody * Triangle = new btRigidBody( TriangleCI );
	bulletObject * TrianglePtr = new bulletObject( Name, Triangle );
	Triangle->setUserPointer( TrianglePtr );
	m_pWorld->addRigidBody( Triangle );
	m_vecRigidBodies.push_back( TrianglePtr );
	return Triangle;
}

btCollisionShape * CPhysics::CreateCustomCollisionShape( std::vector<CModel::SVertex>& vertices, std::vector<DWORD>& indices )
{
	btTriangleMesh * TriangleMesh = new btTriangleMesh( true, false );
	m_vecMeshes.push_back( TriangleMesh );
	for ( unsigned int i = 0; i < indices.size( ) / 3; ++i )
	{
		CModel::SVertex* Vertex1 = &vertices[ indices[ i * 3 + 0 ] ];
		CModel::SVertex* Vertex2 = &vertices[ indices[ i * 3 + 1 ] ];
		CModel::SVertex* Vertex3 = &vertices[ indices[ i * 3 + 2 ] ];
		TriangleMesh->addTriangle(
			btVector3( Vertex1->Position.x, Vertex1->Position.y, Vertex1->Position.z ),
			btVector3( Vertex2->Position.x, Vertex2->Position.y, Vertex2->Position.z ),
			btVector3( Vertex3->Position.x, Vertex3->Position.y, Vertex3->Position.z ), true
		);
	}
	btBvhTriangleMeshShape * TriangleShape = new btBvhTriangleMeshShape( TriangleMesh, true );
	return TriangleShape;
}

std::pair<int, int> CPhysics::CreateMultipleCustomRigidBodies( std::vector<SCreateCustomRigidBody>& RigidBodiesToCreate )
{
	int Start = m_vecRigidBodies.size( );

	for ( unsigned int i = 0; i < RigidBodiesToCreate.size( ); ++i )
	{
		btRigidBody* Body = CreateRigidBody( RigidBodiesToCreate[ i ].mass,
			&RigidBodiesToCreate[ i ].Trans,
			RigidBodiesToCreate[ i ].Shape );
		bulletObject* BodyPtr = new bulletObject( RigidBodiesToCreate[ i ].Name, Body );
		Body->setUserPointer( BodyPtr );
		m_pWorld->addRigidBody( Body );
		m_vecRigidBodies.push_back( BodyPtr );
	}

	return std::make_pair( Start, Start + RigidBodiesToCreate.size( ) );
}

void CPhysics::Shutdown( )
{
	for ( unsigned int i = 0; i < m_vecRigidBodies.size( ); ++i )
	{
		if ( i < m_vecMeshes.size( ) )
		{
			delete m_vecMeshes[ i ];
		}
		if ( i < m_vecCollisionShapes.size( ) )
		{
			delete m_vecCollisionShapes[ i ];
		}
		if ( m_vecRigidBodies[ i ] == 0 )
			continue;
		for ( int j = 0; j < m_vecRigidBodies[ i ]->Body->getNumConstraintRefs( ); ++j )
		{
			btTypedConstraint * Constraint = m_vecRigidBodies[ i ]->Body->getConstraintRef( j );
			m_pWorld->removeConstraint( Constraint );
			m_vecRigidBodies[ i ]->Body->removeConstraintRef( Constraint );
			delete Constraint;
		}
		m_pWorld->removeCollisionObject( m_vecRigidBodies[ i ]->Body );
		delete m_vecRigidBodies[ i ]->Body->getMotionState( );
		delete m_vecRigidBodies[ i ]->Body;
		delete m_vecRigidBodies[ i ];
		m_vecRigidBodies[ i ] = 0;
	}

	delete m_pWorld;

	delete m_pSolver;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
	delete m_pBroadphase;
}

CPhysics::~CPhysics( )
{
	Shutdown( );
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
	btRigidBody * Wall = nullptr;

	if ( First->Body->getUserIndex( ) == PlayerID )
		User = First->Body;
	else if ( First->Body->getUserIndex( ) == CheckpointID )
		Torus = First->Body;
	else if ( First->Body->getUserIndex( ) == WallID )
		Wall = First->Body;
	
	if ( Second->Body->getUserIndex( ) == PlayerID )
		User = Second->Body;
	else if ( Second->Body->getUserIndex( ) == CheckpointID )
		Torus = Second->Body;
	else if ( Second->Body->getUserIndex( ) == WallID )
		Wall = Second->Body;

	if ( User == nullptr )
		return false;
	if ( Torus != nullptr )
	{
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
			User->getAabb( UserMinAABB, UserMaxAABB );
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
	else if ( Wall != nullptr )
	{
		if ( !( ( bulletObject* ) User->getUserPointer( ) )->bUpdateLivesThisFrame )
		{
			( ( bulletObject* ) User->getUserPointer( ) )->bUpdateLivesThisFrame = true;
			( ( bulletObject* ) User->getUserPointer( ) )->Lives = ( ( bulletObject* ) User->getUserPointer( ) )->Lives - 1;
		}
		return true;
	}
	return false;
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
		if ( ( ( bulletObject* ) object->m_vecRigidBodies[ i ]->Body->getUserPointer( ) )->Name == L"Player" )
		{
			Velocity.setX( 0 );
			Velocity.setZ( 0 );
			btScalar VerticalSpeed = Velocity.length2( );
			if ( VerticalSpeed > VerticalSpeedToFly2 )
				( ( bulletObject* ) object->m_vecRigidBodies[ i ]->Body->getUserPointer( ) )->bTouchesTheGround = false;
			else
				( ( bulletObject* ) object->m_vecRigidBodies[ i ]->Body->getUserPointer( ) )->bTouchesTheGround = true;
		}
	}
}