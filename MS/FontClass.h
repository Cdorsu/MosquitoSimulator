#pragma once

#include "Texture.h"
#include "common\standardIncludes.h"
#include "common\windowsIncludes.h"

class FontClass
{
private:
	static constexpr UINT TotalLetters = 95;
private:
	typedef struct SLetter
	{
		float left, right;
		float width;
	} SLetterInfo;
public:
	struct SVertex
	{
		SVertex( float x, float y,
			float u, float v )
			: Pos( x, y, 0.0f ), Tex( u, v ) {};
			DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 Tex;
	};
private:
	SLetter * m_Letters;
	CTexture * m_Texture;
	float m_fFontHeight;
public:
	FontClass( );
	~FontClass( );
public:
	bool Initialize( ID3D11Device* device, LPWSTR lpFontTexture, LPWSTR lpFontInfo, float Height );
	void Build( void * whereto, LPCSTR lpSentence, float x, float y );
	void Shutdown( );
public:
	inline ID3D11ShaderResourceView * GetTexture( ) { return m_Texture->GetTexture( ); };
};

