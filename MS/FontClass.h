#pragma once

#include "Texture.h"
#include "common\standardIncludes.h"
#include "common\windowsIncludes.h"

class CText;

class FontClass sealed
{
	friend class CText;
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
	void Build( _Out_ void * whereto, _Out_ void * indices, _Out_ UINT& VertexCount, _Out_ UINT& IndexCount,
		_In_ LPCSTR lpSentence, _In_ float x, _In_ float y );
	void Shutdown( );
public:
	inline ID3D11ShaderResourceView * GetTexture( ) { return m_Texture->GetTexture( ); };
};

