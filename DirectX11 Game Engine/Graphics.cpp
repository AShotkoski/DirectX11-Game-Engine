#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")

Graphics::Graphics( HWND hWnd )
{
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext 
	);

	// Create render target view
	ID3D11Resource* pBackBuffer = nullptr;
	pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Resource ), reinterpret_cast<void**>( &pBackBuffer ) );
	
	pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView );
	if(pBackBuffer != nullptr )
		pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if ( pDevice != nullptr )
		pDevice->Release();
	if ( pSwapChain != nullptr )
		pSwapChain->Release();
	if ( pContext != nullptr )
		pContext->Release();
	if ( pRenderTargetView != nullptr )
		pRenderTargetView->Release();
}

void Graphics::BeginFrame()
{
}

void Graphics::EndFrame()
{
	float c[ 4 ] = { 1.f,0.5f,0.f,1.f };
	pContext->ClearRenderTargetView( pRenderTargetView, c );
	pSwapChain->Present( 1u, 0u );
}
