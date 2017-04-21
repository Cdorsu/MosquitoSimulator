#pragma once

#define UNICODE

#ifndef UNICODE
#define ENGINE_NAME "Apa Borsec v0.2"
#define GAME_NAME "Mosquito Simulator"
#else // !UNICODE
#define ENGINE_NAME L"Apa Borsec v0.2"
#define GAME_NAME L"Mosquito Simulator"
#endif

#define SAFE_RELEASE( Object ) { if ( (Object) != nullptr ) { (Object)->Release(); (Object) = nullptr; } }

#define DIRECTINPUT_VERSION 0x0800

#define IFFAILED(expr, error) if ( FAILED(expr) ) { OutputDebugString(error); return false; }

#define ALIGN16 __declspec ( align ( 16 ) )