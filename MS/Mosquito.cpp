#include "Mosquito.h"

std::random_device CMosquito::m_RandomDevice;
std::mt19937 CMosquito::m_RandomGenerator = std::mt19937( CMosquito::m_RandomDevice( ) );
std::uniform_real_distribution<float> CMosquito::m_FloatWingAngleDistribution( ( FLOAT ) -D3DX_PI / 8.f, ( FLOAT ) D3DX_PI / 8.f );

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
	for ( unsigned int i = m_numStaticObjects; i < m_vecModels.size( ); ++i )
	{
		CModel * Model = m_vecModels[ i ];
		SAFE_RELEASE( Model->m_VertexBuffer );
		D3D11_BUFFER_DESC buffDesc = { 0 };
		buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		buffDesc.ByteWidth = sizeof( CModel::SVertex ) * 4;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		IFFAILED( device->CreateBuffer( &buffDesc, nullptr, &Model->m_VertexBuffer ),
			L"Couldn't create a vertex buffer" );
	}
	return true;
}

void CMosquito::CalculateAABB( )
{
	m_3fMinAABB = DirectX::XMFLOAT3( FLT_MAX, FLT_MAX, FLT_MAX );
	m_3fMaxAABB = DirectX::XMFLOAT3( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	for ( UINT i = 0; i < m_numStaticObjects; ++i )
	{
		if ( m_vecModels[ i ]->m_3fMinAABB.x < m_3fMinAABB.x )
			m_3fMinAABB.x = m_vecModels[ i ]->m_3fMinAABB.x;
		if ( m_vecModels[ i ]->m_3fMaxAABB.x > m_3fMaxAABB.x )
			m_3fMaxAABB.x = m_vecModels[ i ]->m_3fMaxAABB.x;

		if ( m_vecModels[ i ]->m_3fMinAABB.y < m_3fMinAABB.y )
			m_3fMinAABB.y = m_vecModels[ i ]->m_3fMinAABB.y;
		if ( m_vecModels[ i ]->m_3fMaxAABB.y > m_3fMaxAABB.y )
			m_3fMaxAABB.y = m_vecModels[ i ]->m_3fMaxAABB.y;

		if ( m_vecModels[ i ]->m_3fMinAABB.z < m_3fMinAABB.z )
			m_3fMinAABB.z = m_vecModels[ i ]->m_3fMinAABB.z;
		if ( m_vecModels[ i ]->m_3fMaxAABB.z > m_3fMaxAABB.z )
			m_3fMaxAABB.z = m_vecModels[ i ]->m_3fMaxAABB.z;
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

void CMosquito::UpdateWings( ID3D11DeviceContext * context, DirectX::XMFLOAT3 Direction, bool bAnimateWings )
{
	static HRESULT hr;
	static D3D11_MAPPED_SUBRESOURCE MappedResource;
	DirectX::XMVECTOR Point;
	DirectX::XMMATRIX Rotation;
	DirectX::XMVECTOR Axis = DirectX::XMLoadFloat3( &Direction );
	for ( unsigned int i = m_numStaticObjects; i < m_vecModels.size( ); ++i )
	{
		float Angle = bAnimateWings ? m_FloatWingAngleDistribution( m_RandomGenerator ) : 0.0f;
		Rotation = DirectX::XMMatrixRotationAxis( Axis, Angle );
		hr = context->Map( m_vecModels[ i ]->m_VertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
		if ( FAILED( hr ) )
			return;

		( ( CModel::SVertex* )MappedResource.pData )[ 0 ].Position = m_vecModels[ i ]->m_vecVertices[ 0 ].Position;
		( ( CModel::SVertex* )MappedResource.pData )[ 3 ].Position = m_vecModels[ i ]->m_vecVertices[ 3 ].Position;

		Point = DirectX::XMLoadFloat3( &m_vecModels[ i ]->m_vecVertices[ 1 ].Position );
		Point = DirectX::XMVector3TransformCoord( Point, Rotation );
		DirectX::XMStoreFloat3( &( ( CModel::SVertex* )MappedResource.pData )[ 1 ].Position, Point );

		Point = DirectX::XMLoadFloat3( &m_vecModels[ i ]->m_vecVertices[ 2 ].Position );
		Point = DirectX::XMVector3TransformCoord( Point, Rotation );
		DirectX::XMStoreFloat3( &( ( CModel::SVertex* )MappedResource.pData )[ 2 ].Position, Point );

		context->Unmap( m_vecModels[ i ]->m_VertexBuffer, 0 );
	}
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
