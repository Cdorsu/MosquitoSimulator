#include "Mosquito.h"



CMosquito::CMosquito( )
{
}

bool CMosquito::Initialize( ID3D11Device * device, LPWSTR lpList )
{
	std::wstring word;
	wchar_t ch;
	UINT readCount = 0;

	std::wifstream ifCitire( lpList );

	ifCitire >> word;
	ifCitire.get( ch );
	while ( ch != '\n' )
		ifCitire.get( ch );

	ifCitire >> word;
	while ( readCount < 2 )
	{
		if ( word == L"Static:" )
		{
			ifCitire >> m_numStaticObjects;
			ifCitire >> m_numStaticNoCulledObjects;
			ifCitire >> word;
			for ( unsigned int i = 0; i < m_numStaticObjects; ++i )
			{
				ifCitire >> word;
				if ( word.size( ) < 4 || word == L"#" )
				{
					std::getline( ifCitire, word );
					i--;
					continue;
				}
				CModel* newModel = new CModel( );
				if ( !newModel->Initialize( device,
					const_cast< LPWSTR >( word.c_str( ) ) ) )
					return false;
				m_vecModels.push_back( newModel );
			}
			ifCitire.get( ch );
			while ( ch != '}' )
				ifCitire.get( ch );
		}
		else if ( word == L"Dynamic:" )
		{
			ifCitire >> m_numDynamicObjects;
			ifCitire >> word;
			for ( unsigned int i = 0; i < m_numDynamicObjects; ++i )
			{
				ifCitire >> word;
				if ( word.size( ) < 4 || word == L"#" )
				{
					std::getline( ifCitire, word );
					i--;
					continue;
				}
				CModel* newModel = new CModel( );
				if ( !newModel->Initialize( device,
					const_cast< LPWSTR >( word.c_str( ) ) ) )
					return false;
				m_vecModels.push_back( newModel );
			}
			ifCitire.get( ch );
			while ( ch != '}' )
				ifCitire.get( ch );
		}
		ifCitire >> word;
		readCount++;
	}

	while ( !ifCitire.eof( ) )
	{
		/// Maybe do something with other data
	}
	ifCitire.close( );
	//m_StaticWorld = DirectX::XMMatrixScaling( 0.3f, 0.3f, 0.3f ) * DirectX::XMMatrixTranslation( 0.05f, -0.5f, -0.5f );
	m_StaticWorld = DirectX::XMMatrixIdentity( );
	m_IdentityMatrix = DirectX::XMMatrixScaling( 0.3f, 0.3f, 0.3f );
	for ( unsigned int i = 0; i < m_vecModels.size( ); ++i )
	{
		m_vecModels[ i ]->m_World = DirectX::XMMatrixIdentity( );;
	}
	return true;
}

void CMosquito::CalculateAABB( )
{
	m_3fMinAABB = DirectX::XMFLOAT3( FLT_MAX, FLT_MAX, FLT_MAX );
	m_3fMaxAABB = DirectX::XMFLOAT3( FLT_MIN, FLT_MIN, FLT_MIN );
	for ( UINT i = 0; i < m_vecModels.size( ); ++i )
	{
		if ( m_vecModels[ i ]->m_3fMinAABB.x < m_3fMinAABB.x )
			m_3fMinAABB.x = m_vecModels[ i ]->m_3fMinAABB.x;
		else if ( m_vecModels[ i ]->m_3fMaxAABB.x > m_3fMaxAABB.x )
			m_3fMaxAABB.x = m_vecModels[ i ]->m_3fMinAABB.x;
		if ( m_vecModels[ i ]->m_3fMinAABB.y < m_3fMinAABB.y )
			m_3fMinAABB.y = m_vecModels[ i ]->m_3fMinAABB.y;
		else if ( m_vecModels[ i ]->m_3fMaxAABB.y > m_3fMaxAABB.y )
			m_3fMaxAABB.y = m_vecModels[ i ]->m_3fMinAABB.y;
		if ( m_vecModels[ i ]->m_3fMinAABB.z < m_3fMinAABB.z )
			m_3fMinAABB.z = m_vecModels[ i ]->m_3fMinAABB.z;
		else if ( m_vecModels[ i ]->m_3fMaxAABB.z > m_3fMaxAABB.z )
			m_3fMaxAABB.z = m_vecModels[ i ]->m_3fMinAABB.z;
	}
}

void CMosquito::CalculateCenter( bool bReconstructBuffers )
{
	float distX = ( m_3fMaxAABB.x - m_3fMinAABB.x ) / 2.0f;
	float distY = ( m_3fMaxAABB.y - m_3fMinAABB.y ) / 2.0f;
	float distZ = ( m_3fMaxAABB.z - m_3fMinAABB.z ) / 2.0f;
	m_3fCenter = DirectX::XMFLOAT3( m_3fMaxAABB.x - distX,
		m_3fMaxAABB.y - distY, m_3fMaxAABB.z - distZ );
}

void CMosquito::UpdateWings( )
{
	static float theta = 0.0f;
	theta += 0.2f;
	if ( theta >= 2 * ( FLOAT ) D3DX_PI )
		theta = 0.0f;
	CModel * Wing = m_vecModels[ m_numStaticObjects ];
	Wing->Identity( );
	Wing->RotateY( theta );
	Wing->Translate( -0.1f, 3.05f, 1.49f );
}

void CMosquito::Render( ID3D11DeviceContext * context, UINT objectIndex )
{
	m_vecModels[ objectIndex ]->Render( context );
}

void CMosquito::Shutdown( )
{
	for ( UINT i = 0; i < m_vecModels.size( ); ++i )
	{
		m_vecModels[ i ]->Shutdown( );
		delete m_vecModels[ i ];
		m_vecModels[ i ] = 0;
	}
	m_vecModels.resize( 0 );
}

CMosquito::~CMosquito( )
{
	Shutdown( );
}
