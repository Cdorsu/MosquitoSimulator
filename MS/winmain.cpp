#include "Application.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int iShow )
{
	CApplication *game;
	try
	{
		game = new CApplication( );
		if ( !game->Initialize( hInstance, false ) )
		{
			delete game;
			return 1;
		}
		game->Run( );
		delete game;
		return 0;
	}
	catch ( std::exception & e )
	{
		char buffer[ 5000 ];
		wsprintfA( buffer, "Error occured: %s", e.what( ) );
		MessageBoxA( NULL, buffer, "Error", MB_OK | MB_ICONERROR );

	}
	catch ( ... )
	{
		MessageBox( NULL, L"An unexpected error occured",
			L"Error", MB_OK | MB_ICONERROR );
	}
	return 1;
}