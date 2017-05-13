#pragma once

#include <DirectXMath.h>
#include "common\definitions.h"
#include "common\standardIncludes.h"
#include "common\windowsIncludes.h"

class CViewInterface
{
	std::array<DirectX::XMFLOAT4, 6> m_ViewFrustumPlanes;
public:
	virtual inline DirectX::XMMATRIX& GetView( ) = 0;
	virtual inline DirectX::XMMATRIX& GetProjection( ) = 0;
	virtual inline DirectX::XMFLOAT3 GetCamPos( ) = 0;
public:
	void ConstructFrustum( );
	bool isPointInFrustum( float x, float y, float z );
	bool isAABBCompletelyInFrustum( float minX, float minY, float minZ,
		float maxX, float maxY, float maxZ );
	bool isAABBPartialInFrustum( float minX, float minY, float minZ,
		float maxX, float maxY, float maxZ );
};

