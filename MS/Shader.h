#pragma once

#include "common\definitions.h"
#include "common\DirectXIncludes.h"
#include "common\standardIncludes.h"
#include "Light.h"
#include "Camera.h"

class CShader
{
private:
	std::vector<ID3DBlob*> m_vecBlobs;
public:
	CShader( )
	{
	}
	virtual ~CShader( )
	{
		for ( size_t i = 0; i < m_vecBlobs.size( ); i++ )
		{
			SAFE_RELEASE( m_vecBlobs[ i ] );
		}
	}
public:
	virtual void SetShaders( ID3D11DeviceContext * context ) = 0;
	virtual void Shutdown( ) = 0;
public:
	inline void DrawIndexed( ID3D11DeviceContext * context, UINT indexCount )
	{
		context->DrawIndexed( indexCount, 0, 0 );
	};
	inline void DrawIndexedInstanced( ID3D11DeviceContext * context, UINT indexCount, UINT instanceCount )
	{
		context->DrawIndexedInstanced( indexCount, instanceCount, 0, 0, 0 );
	};
protected:
	inline bool CompileAndCreateVertexShader( ID3D11Device * device,
		_In_ LPWSTR lpFileNameDebug, _In_ LPWSTR lpFileNameRelease, // What file to compile
		_Out_ ID3D11VertexShader ** VertexShader, // What vertex shader to create
		_Out_ ID3DBlob ** ShaderBlob ) // And Input Layout
	{
		HRESULT hr = S_OK;
#if _DEBUG
		ID3DBlob * ErrorBlob;
		hr = D3DX11CompileFromFile( lpFileNameDebug, // Compile this file
			nullptr, nullptr, // No extra files / definitions
			"main", "vs_4_0", // which function to compile and how
			NULL, NULL, // No extra flags
			nullptr, // No thread pump
			ShaderBlob, &ErrorBlob,
			nullptr ); // No thread pump -> this is NULL
		if ( FAILED( hr ) )
		{
			if ( ErrorBlob )
			{
				OutputErrorString( ErrorBlob );
			}
			OutputDebugString( L"Couldn't compile a vertex shader" );
			return false;
		}
#else
		hr = D3DReadFileToBlob( lpFileNameRelease, ShaderBlob );
		IFFAILED( hr, L"Couldn't read content of a vertex shader file (.cso)" );
#endif
		hr = device->CreateVertexShader( ShaderBlob[ 0 ]->GetBufferPointer( ), // Create a vertex shader from this address
			ShaderBlob[ 0 ]->GetBufferSize( ), // To this address
			nullptr, // No linkage
			VertexShader ); // And store it here
		IFFAILED( hr, L"Couldn't create a vertex shader" );
		m_vecBlobs.push_back( *ShaderBlob );
		
		return true;
	};
	inline bool CompileAndCreatePixelShader( ID3D11Device * device,
		_In_ LPWSTR lpFilenameDebug, _In_ LPWSTR lpFilenameRelease, // What file to compile
		_Out_ ID3D11PixelShader ** PixelShader )
	{
		HRESULT hr = S_OK;
		ID3DBlob * ShaderBlob;
#if _DEBUG
		ID3DBlob * ErrorBlob;
		hr = D3DX11CompileFromFile( lpFilenameDebug, // Compile this file
			nullptr, nullptr, // No extra files / definitions
			"main", "ps_4_0", // which function to compile and how
			NULL, NULL, // No extra flags
			nullptr, // No thread pump
			&ShaderBlob, &ErrorBlob,
			nullptr ); // No thread pump -> this is NULL
		if ( FAILED( hr ) )
		{
			if ( ErrorBlob )
			{
				OutputErrorString( ErrorBlob );
			}
			OutputDebugString( L"Couldn't compile a pixel shader" );
			return false;
		}
#else
		hr = D3DReadFileToBlob( lpFilenameRelease, &ShaderBlob );
		IFFAILED( hr, L"Couldn't read content of a pixel shader file (.cso)" );
#endif
		hr = device->CreatePixelShader( ShaderBlob->GetBufferPointer( ),
			ShaderBlob->GetBufferSize( ),
			nullptr,
			PixelShader );
		IFFAILED( hr, L"Couldn't create a pixel shader" );
		m_vecBlobs.push_back( ShaderBlob );
		return true;
	};
	inline static void OutputErrorString( ID3DBlob * Blob, LPWSTR lpFile = L"" )
	{
		wchar_t *start = ( wchar_t* ) Blob->GetBufferPointer( );
		UINT size = Blob->GetBufferSize( );
		if ( lpFile == L"" )
		{
			for ( UINT i = 0; i < size; ++i )
			{
				OutputDebugString( start + i );
			}
		}
		else
		{
			std::wofstream ofOutput( lpFile );
			for ( UINT i = 0; i < size; ++i )
			{
				ofOutput << ( start + i );
			}
		}
	};
};

