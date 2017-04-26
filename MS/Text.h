#pragma once

#include "common\DirectXIncludes.h"
#include "common\windowsIncludes.h"
#include "FontClass.h"

class CText
{
	ID3D11Buffer * m_VertexBuffer;
	ID3D11Buffer * m_IndexBuffer;

private: // Do not create / destroy it, take it from an upper level (CGraphics)
	FontClass * m_Font;

private:
	size_t m_MaxLength;
	UINT m_IndexCount;
	UINT m_VertexCount;
	float m_fWindowWidth;
	float m_fWindowHeight;
	float m_fPreviousX;
	float m_fPreviousY;
	LPCSTR m_lpLastMessage;
public:
	CText( );
	~CText( );
public:
	bool Initialize( ID3D11Device * device, FontClass * Font, size_t maxLength, float WindowWidth, float WindowHeight );
	void Render( ID3D11DeviceContext * context );
	void Update( ID3D11DeviceContext * context, float x, float y, LPCSTR Message );
	void Shutdown( );
public:
	inline UINT GetIndexCount( ) { return m_IndexCount; };
	inline UINT GetVertexCount( ) { return m_VertexCount; };
	inline ID3D11ShaderResourceView * GetTexture( ) { return m_Font->GetTexture( ); };
	inline float GetHeight( ) { return m_Font->m_fFontHeight; };
};

