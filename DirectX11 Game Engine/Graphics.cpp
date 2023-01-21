#include "Graphics.h"
#include "Macros.h"
#include <sstream>

#pragma comment(lib,"d3d11.lib")

Graphics::Graphics( HWND hWnd )
{
	HRESULT hr;

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

	// Use debug flag in d3ddevice creation if in debug mode
	UINT flags = 0u;
#ifndef NDEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	THROW_FAILED_GFX(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	// Create render target view
	ID3D11Resource* pBackBuffer = nullptr;
	THROW_FAILED_GFX(pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Resource ),
											reinterpret_cast<void**>( &pBackBuffer ) ));
	
	THROW_FAILED_GFX(pDevice->CreateRenderTargetView( pBackBuffer, nullptr, &pRenderTargetView ));

	// Cleanup
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
	float c[ 4 ] = { 1.f,0.5f,0.f,1.f };
	pContext->ClearRenderTargetView( pRenderTargetView, c );
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// Present back buffer
	if ( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		// Special throw case if graphics driver crashes
		if ( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			throw GFX_EXCEPT( hr );
		}
	}
}


Graphics::Exception::Exception( int line, const std::string& file, HRESULT hr )
	:
	BaseException( line, file ),
	hr( hr )
{
}

const char* Graphics::Exception::what() const noexcept
{
	std::ostringstream ss;
	ss << BaseException::what() << std::endl
		<< "[Code] " << std::hex << GetErrorCode() << std::dec << '(' << GetErrorCode() << ')' << std::endl
		<< "[Description] " << GetErrorString() << std::endl;
	whatBuffer = ss.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::GetType() const noexcept
{
	return "Caught Graphics Exception:";
}

std::string Graphics::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode( hr );
}

HRESULT Graphics::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::Exception::TranslateErrorCode( HRESULT hRes ) noexcept
{
	char* pMsgBuffer = nullptr;
	DWORD dwMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hRes, 0,
		reinterpret_cast<LPSTR>( &pMsgBuffer ),
		0u, nullptr
	);

	if ( dwMsgLen == 0 )
		return "Error code unknown.";

	std::string strMsg = pMsgBuffer;
	LocalFree( pMsgBuffer );

	return strMsg;
}
