#include "Physics.h"



CPhysics::CPhysics( )
{
}

bool CPhysics::Initialize( CGraphics * GraphicsObject, CInput * InputObject )
{
	m_Graphics = GraphicsObject;
	m_Input = InputObject;

	m_pBroadphase = new btDbvtBroadphase( );
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration( );
	m_pDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );
	m_pSolver = new btSequentialImpulseConstraintSolver( );
	m_pWorld = new btDiscreteDynamicsWorld( m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration );

	btCollisionShape *PlaneShape = new btStaticPlaneShape( btVector3( 0, 1, 0 ), 0 );
	btMotionState *MotionState = new btDefaultMotionState( );
	btVector3 localInertia = btVector3( 0, 0, 0 );
	btRigidBody::btRigidBodyConstructionInfo PlaneCI( 0, MotionState, PlaneShape, localInertia );
	btRigidBody *Plane = new btRigidBody( PlaneCI );
	bulletObject *PlanePtr = new bulletObject( L"Plane", Plane );
	m_pWorld->addRigidBody( Plane );
	m_vecRigidBodies.push_back( PlanePtr );

	btCollisionShape *BoxShape = new btBoxShape( btVector3( 1, 1, 1 ) );
	btMotionState *BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 10, 0 ) ) );
	btRigidBody::btRigidBodyConstructionInfo BoxCI( 50, BoxState, BoxShape );
	btRigidBody *Box = new btRigidBody( BoxCI );
	
	bulletObject *BoxPtr = new bulletObject( L"Box", Box );
	m_pWorld->addRigidBody( Box );
	m_vecRigidBodies.push_back( BoxPtr );

	BoxState = new btDefaultMotionState( btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 15, 0 ) ) );
	localInertia = btVector3( 10, 0, 0 );
	BoxCI = btRigidBody::btRigidBodyConstructionInfo( 50, BoxState, BoxShape, localInertia );
	Box = new btRigidBody( BoxCI );
	BoxPtr = new bulletObject( L"Box", Box );
	m_pWorld->addRigidBody( Box );
	m_vecRigidBodies.push_back( BoxPtr );
	
	m_pWorld->setGravity( btVector3( 0, -10, 0 ) );

#if DEBUG || _DEBUG
	this->setDebugMode( CPhysics::DebugDrawModes::DBG_MAX_DEBUG_DRAW_MODE );
	m_pWorld->setDebugDrawer( this );
#endif
	return true;
}

void CPhysics::Frame( float fFrameTime )
{
	if ( m_Input->isKeyPressed( DIK_H ) )
	{
		m_vecRigidBodies[ 2 ]->Body->activate( true );
		m_vecRigidBodies[ 2 ]->Body->setAngularVelocity( btVector3( 0, 1, 0 ) * 5 );
	}
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
			m_Graphics->RenderPlane( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Box" )
		{
			float matrix[ 16 ];
			btMotionState* motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			m_Graphics->RenderCube( matrix );
		}
		else if ( m_vecRigidBodies[ i ]->Name == L"Torus" )
		{
			float matrix[ 16 ];
			btMotionState* motionState = m_vecRigidBodies[ i ]->Body->getMotionState( );
			btTransform trans;
			motionState->getWorldTransform( trans );
			trans.getOpenGLMatrix( matrix );
			m_Graphics->RenderTorus( matrix );
		}
	}
	if ( m_Input->isKeyPressed( DIK_B ) )
	{
		m_pWorld->debugDrawWorld( );
	}
}

void CPhysics::Shutdown( )
{
	for ( unsigned int i = 0; i < m_vecRigidBodies.size( ); ++i )
	{
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
