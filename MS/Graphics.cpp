#include "Graphics.h"



CGraphics::CGraphics( )
{
}

bool CGraphics::Initialize( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen, CInput * Input )
{
	m_bFullscreen = bFullscreen;
	m_Input = Input;
	m_WindowWidth = WindowWidth;
	m_WindowHeight = WindowHeight;

	m_Input->addSpecialKey( DIK_V );

	m_D3D11 = new CD3D11( );
	if ( !m_D3D11->Initialize( hWnd, WindowWidth, WindowHeight, CamNear, CamFar, bFullscreen ) )
		return false;

	m_DefaultShader = new CDefaultShader( );
	if ( !m_DefaultShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_WorldShader = new CWorldShader( );
	if ( !m_WorldShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_2DShader = new C2DShader( );
	if ( !m_2DShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_3DShader = new C3DShader( );
	if ( !m_3DShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_SkyboxShader = new CSkyboxShader( );
	if ( !m_SkyboxShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_DepthShader = new CDepthShader( );
	if ( !m_DepthShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_DepthShaderEx = new CDepthShader( );
	if ( !m_DepthShaderEx->Initialize( m_D3D11->GetDevice( ), true ) )
		return false;
	m_ShadowShader = new CShadowShader( );
	if ( !m_ShadowShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;

	m_FirstPersonCamera = new CCamera( );
	if ( !m_FirstPersonCamera->InitializeFirstPersonCamera( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, FOV, CamNear, CamFar, m_Input ) )
		return false;
	m_ThirdPersonCamera = new CCamera( );
	if ( !m_ThirdPersonCamera->InitializeThirdPersonCamera( DirectX::XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ),
		DirectX::XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ), DirectX::XMVectorZero( ), DirectX::XMVectorSet( 0.0f, 3.0f, -3.0f, 1.0f ),
		10.0f, ( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, FOV, CamNear, CamFar, m_Input ) )
		return false;
	m_ActiveCamera = m_FirstPersonCamera;

#if _DEBUG || DEBUG
	m_DebugWindow = new CTextureWindow( );
	if ( !m_DebugWindow->Initialize( m_D3D11->GetDevice( ), L"", WindowWidth, WindowHeight, 100, 100 ) )
		return false;
#endif // _DEBUG || DEBUG
	m_MapWindow = new CTextureWindow( );
	if ( !m_MapWindow->Initialize( m_D3D11->GetDevice( ), L"2DArt\\MapTest.png", WindowWidth, WindowHeight, 100, 100 ) )
		return false;
	m_PlayerWindow = new CTextureWindow( );
	if ( !m_PlayerWindow->Initialize( m_D3D11->GetDevice( ), L"2DArt\\Player.png", WindowWidth, WindowHeight, 3, 3 ) )
		return false;
	m_CheckpointWindow = new CTextureWindow( );
	if ( !m_CheckpointWindow->Initialize( m_D3D11->GetDevice( ), L"2DArt\\Checkpoint.png", WindowWidth, WindowHeight, 3, 3 ) )
		return false;

	m_LineManager = new CLineManager( );
	if ( !m_LineManager->Initialize( m_D3D11->GetDevice( ) ) )
		return false;

	m_Cube = new CModel( );
	if ( !m_Cube->Initialize( m_D3D11->GetDevice( ), L"Assets\\Cube.aba" ) )
		return false;
	m_Torus = new CModel( );
	if ( !m_Torus->Initialize( m_D3D11->GetDevice( ), L"Assets\\Torus.aba" ) )
		return false;
	m_Ground = new CModel( );
	if ( !m_Ground->Initialize( m_D3D11->GetDevice( ), L"Assets\\Ground.aba" ) )
		return false;
	m_Font = new FontClass( );
	if ( !m_Font->Initialize( m_D3D11->GetDevice( ), L"Font\\font.dds", L"Font\\font.txt", 16 ) )
		return false;
	m_Font01 = new FontClass( );
	if ( !m_Font01->Initialize( m_D3D11->GetDevice( ), L"Font\\font01.dds", L"Font\\font01.txt", 32 ) )
		return false;

	m_FPSText = new CText( );
	if ( !m_FPSText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		10, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
	m_FrameTimeText = new CText( );
	if ( !m_FrameTimeText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		20, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
	m_ScoreText = new CText( );
	if ( !m_ScoreText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		12, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
#if _DEBUG || DEBUG
	m_DebugText = new CText( );
	if ( !m_DebugText->Initialize( m_D3D11->GetDevice( ), m_Font01,
		300, ( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
#endif // _DEBUG || DEBUG

	m_Skybox = new CSkybox( );
	if ( !m_Skybox->Initialize( m_D3D11->GetDevice( ), L"Assets\\Skymap.dds" ) )
		return false;
#if !(_DEBUG || DEBUG)
	m_Mosquito = new CMosquito( );
	if ( !m_Mosquito->Initialize( m_D3D11->GetDevice( ), L"Assets\\ComplexModels\\Mosquito.abal" ) )
		return false;
	m_Mosquito->CalculateAABB( );
	m_Mosquito->CalculateCenter( );
#endif // !(_DEBUG || DEBUG)
	m_Depthmap = new CRenderTexture( );
	if ( !m_Depthmap->Initialize( m_D3D11->GetDevice( ), SHADOW_WIDTH, SHADOW_HEIGHT,
		0.1f, 1.0f, 1, ( FLOAT ) 1 / ( FLOAT ) 1 ) )
		return false;

	m_Light = new CLight( );
	m_Light->SetDiffuse( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_Light->SetAmbient( utility::SColor( 0.2f, 0.2f, 0.2f, 1.0f ) );
	m_Light->SetDirection( 0.0f, -0.5f, 0.5f );
	m_Light->SetSpecularColor( m_Light->GetDiffuse( ) );
	m_Light->SetSpecularPower( 128.0f );

	m_LightView = new CLightView( );
	m_LightView->SetLookAt( DirectX::XMVectorZero( ) );
	m_LightView->SetPosition( DirectX::XMVectorSet( -3.0f, 6.0f, -4.0f, 1.0f ) );
	m_LightView->SetAmbient( utility::SColor( 0.1f, 0.1f, 0.1f, 1.0f ) );
	m_LightView->SetDiffuse( utility::hexToRGB( 0xFFFFFF ) );
	m_LightView->SetSpecularColor( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_LightView->GenerateProjectionMatrix( D3DX_PI * 0.7f, ( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, CamNear, CamFar );
	m_LightView->GenerateViewMatrix( );

	//auto Center = m_Mosquito->GetCenter( );
	DirectX::XMFLOAT3 Center( 0.0f, 1.0f, 0.0f );

	m_FirstPersonCamera->SetPosition( DirectX::XMVectorSet( Center.x, Center.y, Center.z, 1.0f ) );
	m_ThirdPersonCamera->SetDirection( DirectX::XMVectorSet( Center.x, Center.y, Center.z, 1.0f ) );

	return true;
}

void CGraphics::Update( float fFrameTime, UINT FPS )
{
	//static float Rotation = 0.0f;

	if ( m_Input->isSpecialKeyPressed( DIK_V ) )
		m_ActiveCamera = m_ActiveCamera == m_FirstPersonCamera ? m_ThirdPersonCamera : m_FirstPersonCamera;

	m_FirstPersonCamera->Update( );
	m_FirstPersonCamera->ConstructFrustum( );
	m_ThirdPersonCamera->Update( );
	m_ThirdPersonCamera->ConstructFrustum( );
	m_Skybox->Update( m_ActiveCamera );
#if !(_DEBUG || DEBUG)
	m_Mosquito->UpdateWings( m_D3D11->GetImmediateContext( ),
		m_AdditionalPhysicsInfo.PlayerDirection, m_AdditionalPhysicsInfo.bAnimateWings );
#endif

	/*Rotation += fFrameTime * 0.2f;
	if ( Rotation >= 4 * ( FLOAT ) D3DX_PI )
		Rotation = 0.0f;

	m_FirstPersonCamera->Update( );
	m_FirstPersonCamera->ConstructFrustum( );
	m_ThirdPersonCamera->Update( );
	m_ThirdPersonCamera->ConstructFrustum( );
	m_Skybox->Update( m_ActiveCamera );

	m_Mosquito->UpdateWings( );

	m_Cube->Identity( );
	m_Cube->RotateY( -Rotation );
	m_Cube->Translate( 0.0f, 1.0f, 5.0f );

	m_Torus->Identity( );
	m_Torus->RotateX( ( FLOAT ) D3DX_PI / 2.f );
	m_Torus->RotateY( Rotation );
	m_Torus->Translate( 0.0f, 3.2f, 5.0f );

	m_Ground->Identity( );
	m_Ground->Scale( 50.f, 50.f, 50.f );

	m_Mosquito->Identity( );*/

	char buffer[ 10 ] = { 0 };
	sprintf_s( buffer, "FPS: %d", FPS );
	m_FPSText->Update( m_D3D11->GetImmediateContext( ), 0, 0, buffer );
	char buffer2[ 20 ] = { 0 };
	sprintf_s( buffer2, "Frame time: %.2lf", fFrameTime );
	m_FrameTimeText->Update( m_D3D11->GetImmediateContext( ), 0, m_FPSText->GetHeight( ), buffer2 );
	char buffer3[ 12 ] = { 0 };
	sprintf_s( buffer3, "Score: %d", m_iScore );
	m_ScoreText->Update( m_D3D11->GetImmediateContext( ), 0, m_FPSText->GetHeight( ) * 2, buffer3 );
#if DEBUG || _DEBUG
	char buffer4[ 300 ] = { 0 };
	sprintf_s( buffer4, "DEBUG MODE" );
	m_DebugText->Update( m_D3D11->GetImmediateContext( ), 0,
		m_FPSText->GetHeight( ) * 3, buffer4 );
#endif
}

void CGraphics::RenderLine( DirectX::XMFLOAT3 From, DirectX::XMFLOAT3 To, utility::SColor Color )
{
	m_LineManager->Render( m_D3D11->GetImmediateContext( ),
		From, To );
	m_3DShader->SetData( m_D3D11->GetImmediateContext( ), DirectX::XMMatrixIdentity( ), m_ActiveCamera );
	m_3DShader->SetMaterialData( m_D3D11->GetImmediateContext( ), false, Color, nullptr );
	m_3DShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	m_3DShader->Draw( m_D3D11->GetImmediateContext( ), m_LineManager->GetVertexCount( ) );
}

void CGraphics::Render( )
{
	m_Depthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_Depthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );

	m_D3D11->EnableBackFaceCulling( );

	m_Ground->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShaderEx->Render( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ), m_Ground->GetWorld( ),
		m_LightView, m_Ground->GetTexture( ) );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShaderEx->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_LightView, m_Cube->GetTexture( ) );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_DepthShaderEx->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_LightView, m_Torus->GetTexture( ) );

	/*for ( UINT i = 0; i < m_Mosquito->GetNumberOfObjects( ); ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
	}*/ /// Simple draw
	m_D3D11->DisableCulling( );
	UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
	for ( UINT i = 0; i < NoCulling; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
	}
	m_D3D11->EnableBackFaceCulling( );
	UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
	for ( UINT i = NoCulling; i < StaticObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
	}
	m_D3D11->DisableCulling( );
	UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
	UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
	for ( UINT i = StaticObjects; i < TotalObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
	}

	m_D3D11->EnableBackFaceCulling( );

	m_D3D11->EnableBackBuffer( );
	m_D3D11->EnableDefaultViewPort( );
	BeginScene( );

	m_Ground->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ), m_Ground->GetWorld( ),
		m_ActiveCamera, m_Ground->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_ActiveCamera, m_Cube->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_ActiveCamera, m_Torus->GetMaterial( ), m_Depthmap->GetTexture( ), m_LightView );
	
	/*for ( UINT i = 0; i < m_Mosquito->GetNumberOfObjects( ); ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
			m_LightView );
	}*/ /// Simple draw
	m_D3D11->DisableCulling( );
	NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
	for ( UINT i = 0; i < NoCulling; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
			m_LightView );
	}
	m_D3D11->EnableBackFaceCulling( );
	StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
	for ( UINT i = NoCulling; i < StaticObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
			m_LightView );
	}
	m_D3D11->DisableCulling( );
	DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
	TotalObjects = m_Mosquito->GetNumberOfObjects( );
	for ( UINT i = StaticObjects; i < TotalObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
			m_LightView );
	}

	m_D3D11->EnableDSLessEqual( );

	m_Skybox->Render( m_D3D11->GetImmediateContext( ) );
	m_SkyboxShader->Render( m_D3D11->GetImmediateContext( ), m_Skybox->GetIndexCount( ),
		m_Skybox->GetWorld( ), m_ActiveCamera, m_Skybox->GetTexture( ) );

	m_D3D11->EnableDefaultDSState( );


	m_FPSText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FPSText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FPSText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_FrameTimeText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FrameTimeText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FrameTimeText->GetTexture( ),
		utility::SColor( 1.0f, 0.0f, 0.0f, 1.0f ) );

#if DEBUG || _DEBUG
	m_DebugText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_DebugText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_DebugText->GetTexture( ),
		utility::SColor( 0.0f, 1.0f, 1.0f, 1.0f ) );
#endif
	EndScene( );
}

void CGraphics::RenderScene( )
{
	static DirectX::XMMATRIX WorldMatrix;
	m_D3D11->EnableBackFaceCulling( );
	m_Depthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_Depthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );
	m_DepthShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	for ( auto & iter : m_mwvecObjectsToDraw ) // First pass - Render to depth buffer
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthMap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Cube" )
		{
			m_Cube->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthMap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Torus" )
		{
			m_Torus->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthMap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Mosquito" )
		{
#if !(_DEBUG || DEBUG)
			if ( iter.second[ 0 ].bRenderDepthMap == false )
				continue;
			WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ 0 ]._4x4fWorld );
			m_D3D11->DisableCulling( );
			UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
			for ( UINT i = 0; i < NoCulling; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
			}
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
			}
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
			}

			m_D3D11->EnableBackFaceCulling( );
#endif
		}
		else
		{
			wchar_t buffer[ 500 ];
			swprintf_s( buffer, L"Couldn't draw an object named \"%s\"\n", iter.first.c_str( ) );
			OutputDebugString( buffer );
		}
	}
	m_D3D11->EnableBackBuffer( );
	m_D3D11->EnableDefaultViewPort( );
	m_ShadowShader->SetShaders( m_D3D11->GetImmediateContext() );
	m_ShadowShader->SetLightData( m_D3D11->GetImmediateContext( ), m_LightView, m_Depthmap->GetTexture( ) );
	for ( auto & iter : m_mwvecObjectsToDraw ) // Second pass - render to back buffer
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Ground->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Cube" )
		{
			m_Cube->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Cube->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Torus" )
		{
			m_Torus->Render( m_D3D11->m_d3d11DeviceContext );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Torus->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Mosquito" )
		{
#if !(_DEBUG || DEBUG)
			if ( iter.second[ 0 ].bRenderBackBuffer == false )
				continue;
			WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ 0 ]._4x4fWorld );
			m_D3D11->DisableCulling( );
			UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
			for ( UINT i = 0; i < NoCulling; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
					m_LightView );
			}
			m_D3D11->DisableCulling( );
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
					m_LightView );
			}
			m_D3D11->DisableCulling( );
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					WorldMatrix, m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_Depthmap->GetTexture( ),
					m_LightView );
			}
			m_D3D11->EnableBackFaceCulling( );
#endif
		}
	}
	m_mwvecObjectsToDraw.clear( );
	RenderUI( );
}

void CGraphics::RenderPlayer( DirectX::XMFLOAT3 Position, float * World )
{
	m_FirstPersonCamera->SetPosition( DirectX::XMVectorSet( Position.x, Position.y, Position.z, 1.0f ) );
	m_ThirdPersonCamera->SetDirection( DirectX::XMVectorSet( Position.x, Position.y, Position.z, 1.0f ) );
	m_PlayerX = Position.x;
	m_PlayerZ = Position.z;

	if ( m_ActiveCamera == m_ThirdPersonCamera ) // Render model only if it's in third person camera
	{
		RenderMosquito( World, true );
	}
	else
		RenderMosquito( World, false );
}

void CGraphics::RenderMosquito( float * World, bool drawtoback )
{
	AddObjectToRenderList( L"Mosquito", nullptr, World, true, drawtoback );
}

void CGraphics::RenderPlane( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Plane", m_Ground, World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Plane", m_Ground, World, bIsInLightFrustum, bIsInViewFrustum );
	}
}

void CGraphics::RenderCube( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Cube", m_Cube, World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Cube", m_Ground, World, bIsInLightFrustum, bIsInViewFrustum );
	}
}

void CGraphics::RenderTorus( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	m_CheckpointX = minX;
	m_CheckpointZ = minZ;
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
	{
	//AddObjectToRenderList( L"Torus", m_Torus, World );
		m_Torus->Render( m_D3D11->GetImmediateContext( ) );
		m_3DShader->SetData( m_D3D11->GetImmediateContext( ), DirectX::XMMATRIX( World ), m_ActiveCamera );
		m_3DShader->SetMaterialData( m_D3D11->GetImmediateContext( ), false, utility::SColor( 1.0f ), nullptr );
		m_3DShader->SetShaders( m_D3D11->GetImmediateContext( ) );
		m_3DShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
	}
	else
	{
		bool bIsInViewFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		if ( bIsInViewFrustum )
		{
			m_Torus->Render( m_D3D11->GetImmediateContext( ) );
			m_3DShader->SetData( m_D3D11->GetImmediateContext( ), DirectX::XMMATRIX( World ), m_ActiveCamera );
			m_3DShader->SetMaterialData( m_D3D11->GetImmediateContext( ), false, utility::SColor( 1.0f ), nullptr );
			m_3DShader->SetShaders( m_D3D11->GetImmediateContext( ) );
			m_3DShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
		}
	}

}

void CGraphics::RenderUI( )
{
	m_D3D11->DisableCulling( );

	m_CheckpointWindow->Render( m_D3D11->GetImmediateContext( ),
		m_WindowWidth - 120 + 50 + ( UINT ) m_CheckpointX, 20 + 50 + ( UINT ) m_CheckpointZ );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_CheckpointWindow->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_CheckpointWindow->GetTexture( ) );

	m_PlayerWindow->Render( m_D3D11->GetImmediateContext( ),
		m_WindowWidth - 120 + 50 + ( UINT ) m_PlayerX, 20 + 50 + ( UINT ) m_PlayerZ );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_PlayerWindow->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_PlayerWindow->GetTexture( ) );

	m_MapWindow->Render( m_D3D11->GetImmediateContext( ), m_WindowWidth - 120, 20 );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_MapWindow->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_MapWindow->GetTexture( ) );

	m_D3D11->EnableDSLessEqual( );

	m_Skybox->Render( m_D3D11->GetImmediateContext( ) );
	m_SkyboxShader->Render( m_D3D11->GetImmediateContext( ), m_Skybox->GetIndexCount( ),
		m_Skybox->GetWorld( ), m_ActiveCamera, m_Skybox->GetTexture( ) );

	m_D3D11->EnableDefaultDSState( );


	m_FPSText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FPSText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FPSText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_FrameTimeText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FrameTimeText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FrameTimeText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_ScoreText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_ScoreText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_ScoreText->GetTexture( ),
		utility::SColor( 1.0f, 0.0f, 0.0f, 1.0f ) );

#if DEBUG || _DEBUG
	m_DebugText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_DebugText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_DebugText->GetTexture( ),
		utility::SColor( 0.0f, 1.0f, 1.0f, 1.0f ) );
#endif
}

CGraphics::~CGraphics( )
{
	if ( m_LightView )
	{
		delete m_LightView;
		m_LightView = 0;
	}
	if ( m_Light )
	{
		delete m_Light;
		m_Light = 0;
	}
	if ( m_Depthmap )
	{
		m_Depthmap->Shutdown( );
		delete m_Depthmap;
		m_Depthmap = 0;
	}
	if ( m_Skybox )
	{
		m_Skybox->Shutdown( );
		delete m_Skybox;
		m_Skybox = 0;
	}
	if ( m_Mosquito )
	{
		m_Mosquito->Shutdown( );
		delete m_Mosquito;
		m_Mosquito = 0;
	}
#if DEBUG || _DEBUG
	if ( m_DebugText )
	{
		m_DebugText->Shutdown( );
		delete m_DebugText;
		m_DebugText = 0;
	}
#endif
	if ( m_ScoreText )
	{
		m_ScoreText->Shutdown( );
		delete m_ScoreText;
		m_ScoreText = 0;
	}
	if ( m_FrameTimeText )
	{
		m_FrameTimeText->Shutdown( );
		delete m_FrameTimeText;
		m_FrameTimeText = 0;
	}
	if ( m_FPSText )
	{
		m_FPSText->Shutdown( );
		delete m_FPSText;
		m_FPSText = 0;
	}
	if ( m_Font01 )
	{
		m_Font01->Shutdown( );
		delete m_Font01;
		m_Font01 = 0;
	}
	if ( m_Font )
	{
		m_Font->Shutdown( );
		delete m_Font;
		m_Font = 0;
	}
	if ( m_Ground )
	{
		m_Ground->Shutdown( );
		delete m_Ground;
		m_Ground = 0;
	}
	if ( m_Torus )
	{
		m_Torus->Shutdown( );
		delete m_Torus;
		m_Torus = 0;
	}
	if ( m_Cube )
	{
		m_Cube->Shutdown( );
		delete m_Cube;
		m_Cube = 0;
	}
	if ( m_LineManager )
	{
		m_LineManager->Shutdown( );
		delete m_LineManager;
		m_LineManager = 0;
	}
	if ( m_CheckpointWindow )
	{
		m_CheckpointWindow->Shutdown( );
		delete m_CheckpointWindow;
		m_CheckpointWindow = 0;
	}
	if ( m_PlayerWindow )
	{
		m_PlayerWindow->Shutdown( );
		delete m_PlayerWindow;
		m_PlayerWindow = 0;
	}
	if ( m_MapWindow )
	{
		m_MapWindow->Shutdown( );
		delete m_MapWindow;
		m_MapWindow = 0;
	}
	if ( m_DebugWindow )
	{
		m_DebugWindow->Shutdown( );
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}
	if ( m_ThirdPersonCamera )
	{
		m_ThirdPersonCamera->Shutdown( );
		delete m_ThirdPersonCamera;
		m_ThirdPersonCamera = 0;
	}
	if ( m_FirstPersonCamera )
	{
		m_FirstPersonCamera->Shutdown( );
		delete m_FirstPersonCamera;
		m_FirstPersonCamera = 0;
	}
	if ( m_ShadowShader )
	{
		m_ShadowShader->Shutdown( );
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}
	if ( m_DepthShaderEx )
	{
		m_DepthShaderEx->Shutdown( );
		delete m_DepthShaderEx;
		m_DepthShaderEx = 0;
	}
	if ( m_DepthShader )
	{
		m_SkyboxShader->Shutdown( );
		delete m_SkyboxShader;
		m_SkyboxShader = 0;
	}
	if ( m_SkyboxShader )
	{
		m_SkyboxShader->Shutdown( );
		delete m_SkyboxShader;
		m_SkyboxShader = 0;
	}
	if ( m_3DShader )
	{
		m_3DShader->Shutdown( );
		delete m_3DShader;
		m_3DShader = 0;
	}
	if ( m_2DShader )
	{
		m_2DShader->Shutdown( );
		delete m_2DShader;
		m_2DShader = 0;
	}
	if ( m_WorldShader )
	{
		m_WorldShader->Shutdown( );
		delete m_WorldShader;
		m_WorldShader = 0;
	}
	if ( m_DefaultShader )
	{
		m_DefaultShader->Shutdown( );
		delete m_DefaultShader;
		m_DefaultShader = 0;
	}
	if ( m_D3D11 )
	{
		m_D3D11->Shutdown( );
		delete m_D3D11;
		m_D3D11 = 0;
	}
}
