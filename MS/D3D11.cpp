#include "D3D11.h"



CD3D11::CD3D11(  )
{
	ZeroMemory( this, sizeof( CD3D11 ) );
	m_BackgroundColor = utility::hexToRGB( 0x0 ); // default black screen
}

CD3D11::CD3D11( HWND hWnd, UINT WindowWidth, UINT WindowHeight, bool bFullscreen )
{
	m_BackgroundColor = utility::hexToRGB( 0xFFFF00 );
	HRESULT hr;
	IDXGIFactory * Factory;
	IDXGIAdapter * Adapter;
	IDXGIOutput * Output;
	DXGI_MODE_DESC * Modes;
	DXGI_ADAPTER_DESC adapterDesc;
	UINT nModes, Numerator, Denominator;

	hr = CreateDXGIFactory( __uuidof( IDXGIFactory ), ( void** ) &Factory );
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't create DXGI factory" );

	hr = Factory->EnumAdapters( 0, &Adapter );
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't enum adapters" );

	hr = Adapter->EnumOutputs( 0, &Output );
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't get output from adapter" );

	hr = Output->GetDisplayModeList( 
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, // Get all the displays with this format
		DXGI_ENUM_MODES_INTERLACED, // Flag
		&nModes, // Put the result in here (number of displays)
		NULL ); // We only need the number now
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't get display mode for monitor" );
	Modes = new DXGI_MODE_DESC[ nModes ];
	hr = Output->GetDisplayModeList(
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, // Get all the displays with this format
		DXGI_ENUM_MODES_INTERLACED, // Flag
		&nModes, // Put the result in here (number of displays)
		Modes ); // Put all the results here
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't get display mode for monitor" );
	for ( UINT i = 0; i < nModes; ++i ) // search through all modes a modes which corresponds with our window
	{
		if ( Modes[ i ].Width == WindowWidth && Modes[ i ].Height == WindowHeight )
		{
			Numerator = Modes[ i ].RefreshRate.Numerator; // Get the proper Numerator / Denominator for RefreshRate
			Denominator = Modes[ i ].RefreshRate.Denominator;
		}
	}

	hr = Adapter->GetDesc( &adapterDesc ); // get description of the adapter
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't get adapter description" );

	m_GPUInfo = adapterDesc.Description;
	m_DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory / 1024 / 1024;

	DXGI_MODE_DESC modeDesc; // Description of back buffer
	ZeroMemory( &modeDesc, sizeof( DXGI_MODE_DESC ) );
	modeDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; // Default format
	modeDesc.Width = WindowWidth;
	modeDesc.Height = WindowHeight;
	modeDesc.RefreshRate.Numerator = Numerator; // Numerator for refresh rate (in case of VSYNC)
	modeDesc.RefreshRate.Denominator = Denominator; // Denominator for refresh rate (in case of VSYNC)
	modeDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // We don't care
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // We don't care
	DXGI_SWAP_CHAIN_DESC swapDesc; // Description of swap chain
	ZeroMemory( &swapDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	swapDesc.BufferCount = 1; // buffer count
	swapDesc.BufferDesc = modeDesc; // buffer description
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Use the buffer as a back buffer
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // change between windowed and fullscreen
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = SAMPLE_DESC_COUNT;
	swapDesc.SampleDesc.Quality = SAMPLE_DESC_QUALITY;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // We don't care
	swapDesc.Windowed = bFullscreen ? FALSE : TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL, // Dont' use adapter so this will work on my computer
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // Use GPU
		NULL, // We are using hardare driver type, so we don't need a sofware module
		NULL, // Flags (Nothing special)
		NULL, // Dont' use feature level so this will work on my computer
		NULL, // Dont' use feature level so this will work on my computer
		D3D11_SDK_VERSION, // Standard version 
		&swapDesc, // swap chain description
		&m_SwapChain, // The swap chain to create
		&m_d3d11Device, // The device to create
		NULL, // We don't care about the feature level this function returns
		&m_d3d11DeviceContext ); // The device context
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't create swapchain, device and device context" );

	ID3D11Texture2D * Backbuffer;
	hr = m_SwapChain->GetBuffer( 0, // Get the first back buffer (the only one)
		__uuidof( ID3D11Texture2D ), // Use this kind of texture
		(void**) &Backbuffer ); // And store the information here
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't get back buffer from swap chain" );

	hr = m_d3d11Device->CreateRenderTargetView(
		Backbuffer, // The data we need
		nullptr, // No furhter description
		&m_d3d11RenderTargetView ); // Make this Render Target View
	Backbuffer->Release( ); // Let's keep it clean
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't create Render Target View" );

	ID3D11Texture2D * DSBuffer;
	D3D11_TEXTURE2D_DESC dsbufferDesc = { 0 }; // description for depth stencil view buffer
	dsbufferDesc.ArraySize = 1; // Use only one texture
	dsbufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	dsbufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 - bit depth; 8 - bit stencil
	dsbufferDesc.Width = WindowWidth;
	dsbufferDesc.Height = WindowHeight;
	dsbufferDesc.MipLevels = 1; // Mip level for texture
	dsbufferDesc.SampleDesc.Count = SAMPLE_DESC_COUNT;
	dsbufferDesc.SampleDesc.Quality = SAMPLE_DESC_QUALITY;
	dsbufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; // Use this as usual
	hr = m_d3d11Device->CreateTexture2D(
		&dsbufferDesc, // create a texture like this
		nullptr, // with no initial data
		&DSBuffer ); // and store it here
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't create depth stencil view buffer" );
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	ZeroMemory( &dsViewDesc, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	dsViewDesc.Format = dsbufferDesc.Format; // it's the same format
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D; // just use a texture 2d
	dsViewDesc.Texture2D.MipSlice = 0; // no mip slice (texture detail)
	hr = m_d3d11Device->CreateDepthStencilView(
		DSBuffer, // create a d/s view with data from here
		&dsViewDesc, // with this description
		&m_d3d11DSView ); // and store it here
	if ( FAILED( hr ) )
		throw std::exception( "Couldn't create depth stencil view" );

	m_d3d11DeviceContext->OMSetRenderTargets( 1, &m_d3d11RenderTargetView, m_d3d11DSView );

	delete Modes;
	Factory->Release( );  // We don't need this anymore
	Adapter->Release( ); // We don't need this anymore
	Output->Release( ); // We don't need this anymore

	ZeroMemory( &m_DefaultViewport, sizeof( D3D11_VIEWPORT ) );
	m_DefaultViewport.TopLeftX = 0; // starts at (0,0)
	m_DefaultViewport.TopLeftY = 0; // and 
	m_DefaultViewport.Width = ( FLOAT ) WindowWidth; // ends at 
	m_DefaultViewport.Height = ( FLOAT ) WindowHeight; // (WindowWidth, WindowHeight)
	m_DefaultViewport.MaxDepth = 1.0f; // this viewport has depth
	m_DefaultViewport.MinDepth = 0.0f; // from 0 to 1

	m_d3d11DeviceContext->RSSetViewports( 1, &m_DefaultViewport );
}

void CD3D11::BeginScene( )
{
	m_d3d11DeviceContext->ClearRenderTargetView( m_d3d11RenderTargetView, m_BackgroundColor );
	m_d3d11DeviceContext->ClearDepthStencilView( m_d3d11DSView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH |
		D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void CD3D11::EndScene( )
{
	m_SwapChain->Present( 1, 0 );
}

CD3D11::~CD3D11( )
{
	SAFE_RELEASE( m_d3d11Device );
	SAFE_RELEASE( m_d3d11DeviceContext );
	SAFE_RELEASE( m_d3d11RenderTargetView );
	SAFE_RELEASE( m_SwapChain );
}
