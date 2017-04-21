#pragma once

#include "common\DirectXIncludes.h"

class CTexture sealed
{
	ID3D11ShaderResourceView * m_Texture = nullptr;
public:
	CTexture( );
	~CTexture( );
public:
	bool Initialize( ID3D11Device * device, LPWSTR lpPath );
	void Shutdown( );
public:
	inline ID3D11ShaderResourceView* GetTexture( ) { return m_Texture; };
};

