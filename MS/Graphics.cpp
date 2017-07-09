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
	m_SunShadowShader = new CSunShadowShader( );
	if ( !m_SunShadowShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_AddTexturesShader = new CAddTexturesShader( );
	if ( !m_AddTexturesShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_HorizontalBlurShader = new CHorizontalBlurShader( );
	if ( !m_HorizontalBlurShader->Initialize( m_D3D11->GetDevice( ) ) )
		return false;
	m_VerticalBlurShader = new CVerticalBlurShader( );
	if ( !m_VerticalBlurShader->Initialize( m_D3D11->GetDevice( ) ) )
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
	if ( !m_DebugWindow->Initialize( m_D3D11->GetDevice( ), L"", WindowWidth, WindowHeight, DebugWindowWidthHeight, DebugWindowWidthHeight ) )
		return false;
#endif // _DEBUG || DEBUG
	m_FullscreenWindow = new CTextureWindow( );
	if ( !m_FullscreenWindow->Initialize( m_D3D11->GetDevice( ), L"", WindowWidth, WindowHeight, WindowWidth, WindowHeight ) )
		return false;
	m_MapWindow = new CTextureWindow( );
	if ( !m_MapWindow->Initialize( m_D3D11->GetDevice( ), L"2DArt\\MapTest.png", WindowWidth, WindowHeight, MapWidth, MapWidth ) )
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
	m_Window = new CModel( );
	if ( !m_Window->Initialize( m_D3D11->GetDevice( ), L"Assets\\Window.aba" ) )
		return false;
	m_Ceil = new CModel( );
	if ( !m_Ceil->Initialize( m_D3D11->GetDevice( ), L"Assets\\Ceiling.aba" ) )
		return false;
	m_LightBulb = new CModel( );
	if ( !m_LightBulb->Initialize( m_D3D11->GetDevice( ), L"Assets\\Lightbulb.aba" ) )
		return false;
	m_Table = new CModel( );
	if ( !m_Table->Initialize( m_D3D11->GetDevice( ), L"Assets\\Table.aba" ) )
		return false;
	m_Chair = new CModel( );
	if ( !m_Chair->Initialize( m_D3D11->GetDevice( ), L"Assets\\Chair.aba" ) )
		return false;
	m_Doggo = new CModel( );
	if ( !m_Doggo->Initialize( m_D3D11->GetDevice( ), L"Assets\\Dog.aba" ) )
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
	if ( !m_DebugText->Initialize( m_D3D11->GetDevice( ), m_Font,
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
	m_LightDepthmap = new CRenderTexture( );
	if ( !m_LightDepthmap->Initialize( m_D3D11->GetDevice( ), SHADOW_WIDTH, SHADOW_HEIGHT,
		0.1f, 1.0f, 1, ( FLOAT ) 1 / ( FLOAT ) 1 ) )
		return false;

	m_LightView = new CLightView( );
	m_LightView->SetLookAt( DirectX::XMVectorSet( 0.1f, 0.0f, 0.1f, 1.0f ) );
	m_LightView->SetPosition( DirectX::XMVectorSet( 0.0f, 45.0f, 0.0f, 1.0f ) );
	m_LightView->SetDirection( DirectX::XMVectorSet( 0.0f, -1.0f, 0.01f, 0.0f ) );
	m_LightView->SetAmbient( utility::SColor( 0.1f, 0.1f, 0.1f, 0.1f ) );
	m_LightView->SetDiffuse( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_LightView->SetSpecularColor( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_LightView->GenerateProjectionMatrix( ( FLOAT ) D3DX_PI * 0.5f, ( FLOAT ) WindowWidth / ( FLOAT ) WindowHeight, CamNear, CamFar );
	m_LightView->GenerateViewMatrix( );

	m_SunDepthmap = new CRenderTexture( );
	if ( !m_SunDepthmap->Initialize( m_D3D11->GetDevice( ), SHADOW_WIDTH, SHADOW_HEIGHT,
		CamNear, CamFar, FOV, 1.f ) ) // These are not used
		return false;

	m_SunLightView = new CSunLightView( );
	m_SunLightView->SetDirection( DirectX::XMVector3Normalize(
		DirectX::XMVectorSet( .3f, -1.0f, .3f, 0.0f )
	) );
	m_SunLightView->SetPosition( DirectX::XMVectorSet( -3.0f, 10.0f, -4.0f, 1.0f ) );
	m_SunLightView->SetAmbient( utility::SColor( 0.1f, 0.1f, 0.1f, 0.0f ) );
	m_SunLightView->SetDiffuse( utility::SColor( 0.5f, 0.5f, 0.5f, 0.1f ) );
	m_SunLightView->SetSpecularColor( utility::SColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_SunLightView->GenerateProjectionMatrix( SunWidthHeight, CamNear, CamFar );
	m_SunLightView->GenerateViewMatrix( );

	m_SceneWithSunLight = new CRenderTexture( );
	if ( !m_SceneWithSunLight->Initialize( m_D3D11->GetDevice( ), WindowWidth, WindowHeight,
		CamNear, CamFar, FOV, ( FLOAT ) WindowWidth / WindowHeight ) )
		return false;
	m_SceneWithLight = new CRenderTexture( );
	if ( !m_SceneWithLight->Initialize( m_D3D11->GetDevice( ), WindowWidth, WindowHeight,
		CamNear, CamFar, FOV, ( FLOAT ) WindowWidth / WindowHeight ) )
		return false;

	//auto Center = m_Mosquito->GetCenter( );
	DirectX::XMFLOAT3 Center( 0.0f, 1.0f, 0.0f );

	m_FirstPersonCamera->SetPosition( DirectX::XMVectorSet( Center.x, Center.y, Center.z, 1.0f ) );
	m_ThirdPersonCamera->SetDirection( DirectX::XMVectorSet( Center.x, Center.y, Center.z, 1.0f ) );

	// Menu
	m_MenuOutline = new CTextureWindow( );
	if ( !m_MenuOutline->Initialize( m_D3D11->GetDevice( ),
		L"2DArt\\Menu.png", MenuOutlineWidth, MenuOutlineHeight,
		MenuOutlineWidth, MenuOutlineHeight ) )
		return false;
	m_Cursor = new CTextureWindow( );
	if ( !m_Cursor->Initialize( m_D3D11->GetDevice( ),
		L"2DArt\\Cursor.dds", WindowWidth, WindowHeight,
		32, 32 ) )
		return false;

	m_vecMenuTexts.push_back( new CText( ) );
	if ( !m_vecMenuTexts[0]->Initialize( m_D3D11->GetDevice( ), m_Font01, 12,
		(FLOAT) WindowWidth, (FLOAT) WindowHeight ) )
		return false;
	m_vecMenuTexts[ 0 ]->Update( m_D3D11->GetImmediateContext( ), -25, -20, "Start game" );
	m_vecMenuTexts[ 0 ]->Update( m_D3D11->GetImmediateContext( ), WindowWidth / 2 - m_vecMenuTexts[ 0 ]->GetWidth( ) / 2,
		WindowHeight / 2 - m_vecMenuTexts[ 0 ]->GetHeight( ), "Start game" );

	m_vecMenuTexts.push_back( new CText( ) );
	if ( !m_vecMenuTexts[ 1 ]->Initialize( m_D3D11->GetDevice( ), m_Font01, 10,
		( FLOAT ) WindowWidth, ( FLOAT ) WindowHeight ) )
		return false;
	m_vecMenuTexts[ 1 ]->Update( m_D3D11->GetImmediateContext( ), -20, 0, "Quit game" );
	m_vecMenuTexts[ 1 ]->Update( m_D3D11->GetImmediateContext( ), WindowWidth / 2 - m_vecMenuTexts[ 1 ]->GetWidth( ) / 2,
		WindowHeight / 2 + m_vecMenuTexts[ 0 ]->GetHeight( ), "Quit game" );

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

	/*Lighting updates*/
	/*DirectX::XMFLOAT3 LightPosition = m_SunLightView->GetCamPos( );
	DirectX::XMFLOAT3 CameraDirection = m_ActiveCamera->GetDirection( );
	float Length = sqrtf( CameraDirection.x * CameraDirection.x + CameraDirection.z * CameraDirection.z );
	CameraDirection.x /= Length;
	CameraDirection.z /= Length;
	CameraDirection.x *= SunInFrontOfCamera;
	CameraDirection.z *= SunInFrontOfCamera;
	LightPosition.x += CameraDirection.x;
	LightPosition.z += CameraDirection.z;


	m_SunLightView->SetPosition( DirectX::XMVectorSet(
		LightPosition.x, LightPosition.y, LightPosition.z, 1.0f
	) );

	m_SunLightView->GenerateViewMatrix( );*/

	DirectX::XMFLOAT3 CameraPosition = m_FirstPersonCamera->GetCamPos( );
	DirectX::XMFLOAT3 CameraDirection = m_FirstPersonCamera->GetDirection( );
	DirectX::XMFLOAT3 LightDirection = m_SunLightView->GetDirection( );
	DirectX::XMFLOAT3 LightPosition;
	LightPosition = CameraPosition;
	float Length = sqrtf( CameraDirection.x * CameraDirection.x + CameraDirection.z * CameraDirection.z );
	CameraDirection.x /= Length;
	CameraDirection.z /= Length;
	LightPosition.x += CameraDirection.x * SunInFrontOfCamera;
	LightPosition.y += CameraDirection.y * SunInFrontOfCamera;
	LightPosition.x -= LightDirection.x * SunDistanceToCamera;
	LightPosition.y -= LightDirection.y * SunDistanceToCamera;
	LightPosition.z -= LightDirection.z * SunDistanceToCamera;

	m_SunLightView->SetPosition( DirectX::XMVectorSet(
		LightPosition.x, LightPosition.y, LightPosition.z, 1.0f
	) );
	m_SunLightView->GenerateViewMatrix( );

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
	m_LightDepthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_LightDepthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );

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
		m_ActiveCamera, m_Ground->GetMaterial( ), m_LightDepthmap->GetTexture( ), m_LightView );

	m_Cube->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ), m_Cube->GetWorld( ),
		m_ActiveCamera, m_Cube->GetMaterial( ), m_LightDepthmap->GetTexture( ), m_LightView );

	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ), m_Torus->GetWorld( ),
		m_ActiveCamera, m_Torus->GetMaterial( ), m_LightDepthmap->GetTexture( ), m_LightView );
	
	/*for ( UINT i = 0; i < m_Mosquito->GetNumberOfObjects( ); ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_LightDepthmap->GetTexture( ),
			m_LightView );
	}*/ /// Simple draw
	m_D3D11->DisableCulling( );
	NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
	for ( UINT i = 0; i < NoCulling; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_LightDepthmap->GetTexture( ),
			m_LightView );
	}
	m_D3D11->EnableBackFaceCulling( );
	StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
	for ( UINT i = NoCulling; i < StaticObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_LightDepthmap->GetTexture( ),
			m_LightView );
	}
	m_D3D11->DisableCulling( );
	DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
	TotalObjects = m_Mosquito->GetNumberOfObjects( );
	for ( UINT i = StaticObjects; i < TotalObjects; ++i )
	{
		m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
		m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
			m_Mosquito->GetModelWorld( i ), m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_LightDepthmap->GetTexture( ),
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
#pragma region Render to depthmap
	m_D3D11->EnableBackFaceCulling( );
#if defined USE_LIGHT
	m_LightDepthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_LightDepthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );
	m_DepthShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	for ( auto & iter : m_mwvecObjectsToDraw ) // First pass - Render to depth buffer
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Cube" )
		{
			m_Cube->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Torus" )
		{
			m_Torus->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Window" )
		{
			m_Window->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderDepthmap )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_D3D11->DisableCulling( );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Window->GetIndexCount( ) );
				m_D3D11->EnableBackFaceCulling( );
			}
		}
		else if ( iter.first == L"Ceiling" )
		{
			// Don't draw it
		}
		else if ( iter.first == L"LightBulb" )
		{
			 // Don't draw it
		}
		else if ( iter.first == L"Table" )
		{
			m_Table->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Table->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Chair" )
		{
			m_Chair->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Chair->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Doggo" )
		{
			m_Doggo->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderDepthmap == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Doggo->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Mosquito" )
		{
#if !(_DEBUG || DEBUG)
			if ( iter.second[ 0 ].bRenderDepthmap == false )
				continue;
			WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ 0 ]._4x4fWorld );
			m_D3D11->DisableCulling( );
			UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
			for ( UINT i = 0; i < NoCulling; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				//m_DepthShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
					//WorldMatrix, m_LightView, m_Mosquito->GetModel( i )->GetTexture( ) );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_LightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
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
#endif

#if defined USE_SUN_LIGHT
	m_SunDepthmap->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_SunDepthmap->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );
	m_DepthShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	for ( auto & iter : m_mwvecObjectsToDraw )
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderSunDepthmap )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Cube" )
		{
			m_Cube->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderSunDepthmap )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Torus" )
		{
			m_Torus->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderSunDepthmap )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Window" )
		{
			m_Window->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderSunDepthmap )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_D3D11->EnableFrontFaceCulling( );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Window->GetIndexCount( ) );
				m_D3D11->EnableBackFaceCulling( );
			}
		}
		else if ( iter.first == L"Mosquito" )
		{
#if !(_DEBUG || DEBUG)
			if ( !iter.second[ 0 ].bRenderSunDepthmap )
				continue;
			WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ 0 ]._4x4fWorld );
			m_D3D11->DisableCulling( );
			UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
			for ( UINT i = 0; i < NoCulling; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_DepthShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_SunLightView );
				m_DepthShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}

			m_D3D11->EnableBackFaceCulling( );
#endif
		}
	}
#endif
#pragma endregion

#if defined USE_SUN_LIGHT

	m_SceneWithSunLight->SetRenderTarget( m_D3D11->GetImmediateContext( ) );
	m_SceneWithSunLight->BeginScene( m_D3D11->GetImmediateContext( ), utility::hexToRGB( 0x0 ) );
	m_SunShadowShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	m_SunShadowShader->SetLightData( m_D3D11->GetImmediateContext( ), m_SunLightView, m_SunDepthmap->GetTexture( ) );
	for ( auto & iter : m_mwvecObjectsToDraw ) // Second pass - render to back buffer
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Ground->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ground->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Cube" )
		{
			m_Cube->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Cube->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Cube->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Torus" )
		{
			m_Torus->Render( m_D3D11->GetImmediateContext() );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( iter.second[ i ].bRenderBackBuffer == false )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Torus->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Window" )
		{
			m_Window->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Window->GetMaterial( ) );
				m_D3D11->DisableCulling( );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Window->GetIndexCount( ) );
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
				/*m_ShadowShader->Render( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ),
				WorldMatrix, m_ActiveCamera, m_Mosquito->GetModel( i )->GetMaterial( ), m_LightDepthmap->GetTexture( ),
				m_LightView );*/
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->DisableCulling( );
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->DisableCulling( );
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_SunShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_SunShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_SunShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->EnableBackFaceCulling( );
#endif
		}
	}
#endif

#if defined USE_LIGHT
	m_D3D11->EnableBackBuffer( );
	m_D3D11->EnableDefaultViewPort( );
	m_ShadowShader->SetShaders( m_D3D11->GetImmediateContext( ) );
	m_ShadowShader->SetLightData( m_D3D11->GetImmediateContext( ), m_LightView, m_LightDepthmap->GetTexture( ) );
	for ( auto & iter : m_mwvecObjectsToDraw ) // Second pass - render to back buffer
	{
		if ( iter.first == L"Plane" )
		{
			m_Ground->Render( m_D3D11->GetImmediateContext() );
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
			m_Cube->Render( m_D3D11->GetImmediateContext() );
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
			m_Torus->Render( m_D3D11->GetImmediateContext() );
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
		else if ( iter.first == L"Window" )
		{
			m_Window->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Window->GetMaterial( ) );
				m_D3D11->DisableCulling( );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Window->GetIndexCount( ) );
				m_D3D11->EnableBackFaceCulling( );
			}
		}
		else if ( iter.first == L"Ceiling" )
		{
			m_Ceil->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Ceil->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Ceil->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"LightBulb" )
		{
			m_LightBulb->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_LightBulb->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_LightBulb->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Table" )
		{
			m_Table->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Table->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Table->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Chair" )
		{
			m_Chair->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Chair->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Chair->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Doggo" )
		{
			m_Doggo->Render( m_D3D11->GetImmediateContext( ) );
			for ( UINT i = 0; i < iter.second.size( ); ++i )
			{
				if ( !iter.second[ i ].bRenderBackBuffer )
					continue;
				WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ i ]._4x4fWorld );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Doggo->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Doggo->GetIndexCount( ) );
			}
		}
		else if ( iter.first == L"Mosquito" )
		{
#if !( _DEBUG || DEBUG )
			if ( iter.second[ 0 ].bRenderBackBuffer == false )
				continue;
			WorldMatrix = DirectX::XMLoadFloat4x4( &iter.second[ 0 ]._4x4fWorld );
			m_D3D11->DisableCulling( );
			UINT NoCulling = m_Mosquito->GetNumberOfStaticObjectsDrawnWithNoCulling( );
			for ( UINT i = 0; i < NoCulling; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->DisableCulling( );
			UINT StaticObjects = m_Mosquito->GetNumberOfStaticObjects( );
			for ( UINT i = NoCulling; i < StaticObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->DisableCulling( );
			UINT DynamicObjects = m_Mosquito->GetNumberOfDynamicObjects( );
			UINT TotalObjects = m_Mosquito->GetNumberOfObjects( );
			for ( UINT i = StaticObjects; i < TotalObjects; ++i )
			{
				m_Mosquito->Render( m_D3D11->GetImmediateContext( ), i );
				m_ShadowShader->SetData( m_D3D11->GetImmediateContext( ), WorldMatrix, m_ActiveCamera );
				m_ShadowShader->SetMaterialData( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetMaterial( ) );
				m_ShadowShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Mosquito->GetModel( i )->GetIndexCount( ) );
			}
			m_D3D11->EnableBackFaceCulling( );
#endif
		}
	}
#endif

	m_mwvecObjectsToDraw.clear( );
	RenderUI( );
}

void CGraphics::RenderPlayer( DirectX::XMFLOAT3 Position, float * World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	m_FirstPersonCamera->SetPosition( DirectX::XMVectorSet( Position.x, Position.y, Position.z, 1.0f ) );
	m_ThirdPersonCamera->SetDirection( DirectX::XMVectorSet( Position.x, Position.y, Position.z, 1.0f ) );
	m_PlayerX = Position.x;
	m_PlayerZ = Position.z;

	m_SunLightView->GenerateViewMatrix( );

	if ( m_ActiveCamera == m_ThirdPersonCamera ) // Render model only if it's in third person camera
	{
		RenderMosquito( World, true );
	}
	else
		RenderMosquito( World, false );
}

void CGraphics::RenderMosquito( float * World, bool drawtoback,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 )
	{
		AddObjectToRenderList( L"Mosquito", World, true, drawtoback, true );
	}
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Mosquito", World, bIsInLightFrustum, drawtoback, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderGround( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Plane", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Plane", World, bIsInLightFrustum, bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderCeiling( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Ceiling", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Ceiling", World, bIsInLightFrustum, bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderCube( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Cube", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Cube", World, bIsInLightFrustum, bIsInViewFrustum, bIsInSunLightFrustum );
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
		/*m_Torus->Render( m_D3D11->GetImmediateContext( ) );
		m_3DShader->SetData( m_D3D11->GetImmediateContext( ), DirectX::XMMATRIX( World ), m_ActiveCamera );
		m_3DShader->SetMaterialData( m_D3D11->GetImmediateContext( ), false, utility::SColor( 1.0f ), nullptr );
		m_3DShader->SetShaders( m_D3D11->GetImmediateContext( ) );
		m_3DShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );*/
		AddObjectToRenderList( L"Torus", World );
	}
	else
	{
		//bool bIsInViewFrustum;
		//bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		//if ( bIsInViewFrustum )
		//{
		//	m_Torus->Render( m_D3D11->GetImmediateContext( ) );
		//	m_3DShader->SetData( m_D3D11->GetImmediateContext( ), DirectX::XMMATRIX( World ), m_ActiveCamera );
		//	m_3DShader->SetMaterialData( m_D3D11->GetImmediateContext( ), false, utility::SColor( 1.0f ), nullptr );
		//	m_3DShader->SetShaders( m_D3D11->GetImmediateContext( ) );
		//	m_3DShader->DrawIndexed( m_D3D11->GetImmediateContext( ), m_Torus->GetIndexCount( ) );
		//}
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Torus", World, bIsInLightFrustum, bIsInViewFrustum, bIsInSunLightFrustum );
	}

}

void CGraphics::RenderWall( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Window", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Window", World, bIsInLightFrustum, bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderLightBulb( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	DirectX::XMMATRIX XMWorld( World );
	DirectX::XMVECTOR Direction = DirectX::XMVectorSet( 0.0f, -1.0f, 0.01f, 0.0f );
	Direction = DirectX::XMVector4Transform( Direction, XMWorld );
	m_LightView->SetDirection( Direction );
	Direction = DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f );
	Direction = DirectX::XMVector3TransformCoord( Direction, XMWorld );
	m_LightView->SetPosition( Direction );

	m_LightView->GenerateViewMatrix( );


	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"LightBulb", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"LightBulb", World, bIsInLightFrustum,
			bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderTable( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Table", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Table", World, bIsInLightFrustum,
			bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderChair( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Chair", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Chair", World, bIsInLightFrustum,
			bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderDoggo( float* World,
	float minX, float minY, float minZ,
	float maxX, float maxY, float maxZ )
{
	if ( minX == 0 && minY == 0 && minZ == 0 &&
		maxX == 0 && maxY == 0 && maxZ == 0 ) // Doesn't have an AABB? Just Render it
		AddObjectToRenderList( L"Doggo", World );
	else
	{
		bool bIsInViewFrustum, bIsInLightFrustum, bIsInSunLightFrustum;
		bIsInViewFrustum = m_ActiveCamera->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInLightFrustum = m_LightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		bIsInSunLightFrustum = m_SunLightView->isAABBPartialInFrustum( minX, minY, minZ, maxX, maxY, maxZ );
		AddObjectToRenderList( L"Doggo", World, bIsInLightFrustum,
			bIsInViewFrustum, bIsInSunLightFrustum );
	}
}

void CGraphics::RenderUI( )
{
	m_D3D11->DisableCulling( );

	m_CheckpointWindow->Render( m_D3D11->GetImmediateContext( ),
		m_WindowWidth - DistanceFromRightWindowLeftMap + MapWidthOver2 + ( UINT ) m_CheckpointX,
		DistanceFromTopToTopMap + MapWidthOver2 + ( UINT ) m_CheckpointZ );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_CheckpointWindow->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_CheckpointWindow->GetTexture( ) );

	m_PlayerWindow->Render( m_D3D11->GetImmediateContext( ),
		m_WindowWidth - DistanceFromRightWindowLeftMap + MapWidthOver2 + ( UINT ) m_PlayerX,
		DistanceFromTopToTopMap + MapWidthOver2 + ( UINT ) m_PlayerZ );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_PlayerWindow->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_PlayerWindow->GetTexture( ) );

	m_MapWindow->Render( m_D3D11->GetImmediateContext( ), m_WindowWidth - DistanceFromRightWindowLeftMap, DistanceFromTopToTopMap );
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

void CGraphics::RenderMenu( float fFrameTime, UINT FPS )
{
	/*Update part*/
	m_fCursorX += m_Input->GetHorizontalMouseMove( );
	m_fCursorY += m_Input->GetVerticalMouseMove( );
	char buffer[ 10 ] = { 0 };
	sprintf_s( buffer, "FPS: %d", FPS );
	m_FPSText->Update( m_D3D11->GetImmediateContext( ), 0, 0, buffer );
	char buffer2[ 20 ] = { 0 };
	sprintf_s( buffer2, "Frame time: %.2lf", fFrameTime );
	m_FrameTimeText->Update( m_D3D11->GetImmediateContext( ), 0, m_FPSText->GetHeight( ), buffer2 );
#if DEBUG || _DEBUG
	char buffer3[ 300 ] = { 0 };
	sprintf_s( buffer3, "DEBUG MODE" );
	m_DebugText->Update( m_D3D11->GetImmediateContext( ), 0,
		m_FPSText->GetHeight( ) * 3, buffer3 );
#endif
	m_MenuSelected = [ & ]
	{
		for ( UINT i = 0; i < m_vecMenuTexts.size( ); ++i )
		{
			if ( m_fCursorX < m_vecMenuTexts[ i ]->GetXPosition( ) + m_vecMenuTexts[ i ]->GetWidth( ) &&
				m_fCursorX > m_vecMenuTexts[ i ]->GetXPosition( ) )
				if ( m_fCursorY < m_vecMenuTexts[ i ]->GetYPosition( ) + m_vecMenuTexts[ i ]->GetHeight( ) &&
					m_fCursorY > m_vecMenuTexts[ i ]->GetYPosition( ) )
					return i;
			
		}
		return 0xffffffff;
	}( );
	
	/*Render part*/
	m_FPSText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FPSText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FPSText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );

	m_FrameTimeText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_FrameTimeText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_FrameTimeText->GetTexture( ),
		utility::SColor( 1.0f, 1.0f, 0.0f, 1.0f ) );


	m_D3D11->EnableAlphaTransparencyBlendingState( );
	m_MenuOutline->Render( m_D3D11->GetImmediateContext( ), 0, 0 );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_MenuOutline->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_MenuOutline->GetTexture( ) );
	m_D3D11->EnableDefaultBlendingState( );
	
	m_Cursor->Render( m_D3D11->GetImmediateContext( ), ( UINT ) m_fCursorX, ( UINT ) m_fCursorY );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_Cursor->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_Cursor->GetTexture( ) );

	for ( UINT i = 0; i < m_vecMenuTexts.size( ); ++i )
	{
		m_vecMenuTexts[ i ]->Render( m_D3D11->GetImmediateContext( ) );
		if ( i == m_MenuSelected )
			m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_vecMenuTexts[ i ]->GetIndexCount( ),
				m_D3D11->GetOrthoMatrix( ), m_vecMenuTexts[ i ]->GetTexture( ), utility::hexToRGB( 0xFFFF00 ) );
		else
			m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_vecMenuTexts[ i ]->GetIndexCount( ),
				m_D3D11->GetOrthoMatrix( ), m_vecMenuTexts[ i ]->GetTexture( ) );
	}

#if DEBUG || _DEBUG
	m_DebugText->Render( m_D3D11->GetImmediateContext( ) );
	m_2DShader->Render( m_D3D11->GetImmediateContext( ), m_DebugText->GetIndexCount( ),
		m_D3D11->GetOrthoMatrix( ), m_DebugText->GetTexture( ),
		utility::SColor( 0.0f, 1.0f, 1.0f, 1.0f ) );
#endif

}

void CGraphics::Shutdown( )
{
	for ( UINT i = 0; i < m_vecMenuTexts.size( ); ++i )
	{
		if ( m_vecMenuTexts[ i ] )
		{
			m_vecMenuTexts[ i ]->Shutdown( );
			delete m_vecMenuTexts[ i ];
			m_vecMenuTexts[ i ] = 0;
		}
	}
	if ( m_Cursor )
	{
		m_Cursor->Shutdown( );
		delete m_Cursor;
		m_Cursor = 0;
	}
	if ( m_MenuOutline )
	{
		m_MenuOutline->Shutdown( );
		delete m_MenuOutline;
		m_MenuOutline = 0;
	}
	if ( m_SceneWithLight )
	{
		m_SceneWithLight->Shutdown( );
		delete m_SceneWithLight;
		m_SceneWithLight = 0;
	}
	if ( m_SceneWithSunLight )
	{
		m_SceneWithSunLight->Shutdown( );
		delete m_SceneWithSunLight;
		m_SceneWithSunLight = 0;
	}
	if ( m_SunLightView )
	{
		delete m_SunLightView;
		m_SunLightView = 0;
	}
	if ( m_SunDepthmap )
	{
		m_SunDepthmap->Shutdown( );
		delete m_SunDepthmap;
		m_SunDepthmap = 0;
	}
	if ( m_LightView )
	{
		delete m_LightView;
		m_LightView = 0;
	}
	if ( m_LightDepthmap )
	{
		m_LightDepthmap->Shutdown( );
		delete m_LightDepthmap;
		m_LightDepthmap = 0;
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
	if ( m_Doggo )
	{
		m_Doggo->Shutdown( );
		delete m_Doggo;
		m_Doggo = 0;
	}
	if ( m_Chair )
	{
		m_Chair->Shutdown( );
		delete m_Chair;
		m_Chair = 0;
	}
	if ( m_Table )
	{
		m_Table->Shutdown( );
		delete m_Table;
		m_Table = 0;
	}
	if ( m_LightBulb )
	{
		m_LightBulb->Shutdown( );
		delete m_LightBulb;
		m_LightBulb = 0;
	}
	if ( m_Ceil )
	{
		m_Ceil->Shutdown( );
		delete m_Ceil;
		m_Ceil = 0;
	}
	if ( m_Window )
	{
		m_Window->Shutdown( );
		delete m_Window;
		m_Window = 0;
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
	if ( m_FullscreenWindow )
	{
		m_FullscreenWindow->Shutdown( );
		delete m_FullscreenWindow;
		m_FullscreenWindow = 0;
	}
#if _DEBUG || DEBUG
	if ( m_DebugWindow )
	{
		m_DebugWindow->Shutdown( );
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}
#endif
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
	if ( m_VerticalBlurShader )
	{
		m_VerticalBlurShader->Shutdown( );
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}
	if ( m_HorizontalBlurShader )
	{
		m_HorizontalBlurShader->Shutdown( );
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}
	if ( m_AddTexturesShader )
	{
		m_AddTexturesShader->Shutdown( );
		delete m_AddTexturesShader;
		m_AddTexturesShader = 0;
	}
	if ( m_SunShadowShader )
	{
		m_SunShadowShader->Shutdown( );
		delete m_SunShadowShader;
		m_SunShadowShader = 0;
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

CGraphics::~CGraphics( )
{
	Shutdown( );
}
