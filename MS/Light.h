#pragma once

#include "common\DirectXIncludes.h"
#include "common\utility.h"

class CLight
{
	DirectX::XMFLOAT3 m_Direction;
	utility::SColor m_Diffuse;
	utility::SColor m_Ambient;
public:

	CLight( )
	{
		ZeroMemory( this, sizeof( CLight ) );
	}

	inline void SetDirection( float x, float y, float z )
	{
		m_Direction = DirectX::XMFLOAT3( x, y, z );
	}

	inline void SetDiffuse( utility::SColor& newDiffuse )
	{
		m_Diffuse = newDiffuse;
	}

	inline void SetAmbient( utility::SColor& newAmbient )
	{
		m_Ambient = newAmbient;
	}

	inline DirectX::XMFLOAT3 GetDirection( )
	{
		return m_Direction;
	}

	inline utility::SColor GetDiffuse( )
	{
		return m_Diffuse;
	}

	inline utility::SColor GetAmbient( )
	{
		return m_Ambient;
	}

	~CLight( )
	{ }
};

