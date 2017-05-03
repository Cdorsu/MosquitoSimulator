#pragma once

#include "common\DirectXIncludes.h"
#include "common\standardIncludes.h"

class CInput sealed
{
	IDirectInput8 * m_DirectInput;
	IDirectInputDevice8 * m_Keyboard;
	IDirectInputDevice8 * m_Mouse;
	DIMOUSESTATE m_currentMouseState;
	DIMOUSESTATE m_lastMouseState;
	BYTE m_currentKeyboard[ 256 ];
	BYTE m_lastKeyboard[ 256 ];
	std::vector<BYTE> m_vecSpecialKeys;
public:
	CInput( );
	CInput( CInput const& ) = delete;
	~CInput( );
public:
	bool Initialize( HINSTANCE hInstance, HWND hWnd );
	void Shutdown( );
public:
	void Frame( );
	bool isKeyPressed( BYTE key );
	bool isSpecialKeyPressed( BYTE key );
public: // Try not to use these in runtime
	bool addSpecialKey( BYTE key );
	bool removeSpecialKey( BYTE key );
public:
	float GetHorizontalMouseMove( )
	{
		return ( float ) m_currentMouseState.lX;
	}
	float GetVerticalMouseMove( )
	{
		return ( float ) m_currentMouseState.lY;
	}
	float GetMouseWheelMove( )
	{
		return ( float ) m_currentMouseState.lZ;
	}
};

