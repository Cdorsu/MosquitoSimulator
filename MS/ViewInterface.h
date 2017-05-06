#pragma once

#include <DirectXMath.h>
#include "common\definitions.h"

class CViewInterface
{
public:
	virtual inline DirectX::XMMATRIX& GetView( ) = 0;
	virtual inline DirectX::XMMATRIX& GetProjection( ) = 0;
	virtual inline DirectX::XMFLOAT3 GetCamPos( ) = 0;
};

