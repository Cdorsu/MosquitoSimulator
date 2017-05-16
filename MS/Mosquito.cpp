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
			ifCitire >> word;
			OutputDebugString( L"Static objects:\n" );
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
	m_StaticWorld = DirectX::XMMatrixScaling( 0.3f, 0.3, 0.3f ) * DirectX::XMMatrixTranslation( 0.05f, -0.5f, -0.5f );
	return true;
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
