#pragma once

#include <DirectXMath.h>

class CViewInterface
{
public:
	virtual DirectX::XMMATRIX& GetView( ) = 0;
	virtual	DirectX::XMMATRIX& GetProjection( ) = 0;
	virtual DirectX::XMFLOAT3 GetCamPos( ) = 0;
};

