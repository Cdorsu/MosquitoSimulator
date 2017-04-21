#pragma once

#include "definitions.h"

#pragma comment (lib, "D3D11.lib")
#if _DEBUG
#pragma comment (lib, "D3DX11d.lib")
#else
#pragma comment (lib, "D3DX11.lib")
#endif
#pragma comment (lib, "D3D10.lib")
#pragma comment (lib, "D3DX10.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "D3Dcompiler.lib")

#include <d3d11.h>
#include <d3dx11.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <dinput.h>
#include <DXGI.h>
#include <D3Dcompiler.h>