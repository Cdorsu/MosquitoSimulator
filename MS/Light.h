#pragma once

#include "common\DirectXIncludes.h"
#include "common\utility.h"

class CLight
{
	DirectX::XMFLOAT3 m_Direction;
	utility::SColor m_Diffuse;
	utility::SColor m_Ambient;
	utility::SColor m_SpecularColor;
	float m_fSpecularPower;
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

	inline void SetSpecularColor( utility::SColor& newSpecular )
	{
		m_SpecularColor = newSpecular;
	}

	inline void SetSpecularPower( float power )
	{
		m_fSpecularPower = power;
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

	inline utility::SColor GetSpecularColor( )
	{
		return m_SpecularColor;
	}

	inline float GetSpecularPower( )
	{
		return m_fSpecularPower;
	}

	~CLight( )
	{ }
};

