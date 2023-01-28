#include "Graphics.h"
#include "Macros.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11sdklayers.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace WRL = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics( HWND hWnd )
	:
	projection(DirectX::XMMatrixIdentity())
{
	// Used for erro chedcking
	HRESULT hr;
	
	// Get window dimensions
	RECT clientRect;
	if ( GetClientRect( hWnd, &clientRect ) == 0 )
		throw std::runtime_error( "Error getting client rect.\n" );

	// Setup SwapChain parameters
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width                   = 0;
	sd.BufferDesc.Height                  = 0;
	sd.BufferDesc.RefreshRate.Numerator   = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count                   = 1;
	sd.SampleDesc.Quality                 = 0;
	sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount                        = 1;
	sd.OutputWindow                       = hWnd;
	sd.Windowed                           = true;
	sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags                              = 0;

	// Use debug flag in d3ddevice creation if in debug mode
	UINT flags = 0u;
#ifndef NDEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create d3d device and swap chain
	THROW_FAILED_GFX( D3D11CreateDeviceAndSwapChain(
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
		&pContext ) );

	// IMGUI
	ImGui_ImplDX11_Init( pDevice.Get(), pContext.Get() );

	// Create render target view
	WRL::ComPtr<ID3D11Resource> pBackBuffer;

	THROW_FAILED_GFX(pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Resource ), &pBackBuffer  ));
	
	THROW_FAILED_GFX(pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pRenderTargetView));

	/************ Create depth / stencil buffer ************/

	// Create depth/stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable              = true;
	dsDesc.DepthFunc                = D3D11_COMPARISON_LESS;
	dsDesc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.StencilEnable            = false;
	WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	THROW_FAILED_GFX( pDevice->CreateDepthStencilState( &dsDesc, &pDepthStencilState ) );

	// Set state
	pContext->OMSetDepthStencilState( pDepthStencilState.Get(), 1u );

	// Create texture for depth/stencil
	WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC         descDepth = {};
	descDepth.Width                        = clientRect.right;
	descDepth.Height                       = clientRect.bottom;
	descDepth.MipLevels                    = 1u;
	descDepth.ArraySize                    = 1u;
	descDepth.Format                       = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count             = 1u;
	descDepth.SampleDesc.Quality           = 0u;
	descDepth.Usage                        = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags                    = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags               = 0u;
	descDepth.MiscFlags                    = 0u;
	THROW_FAILED_GFX(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// Depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC DSviewDesc = {};
	DSviewDesc.Format                        = DXGI_FORMAT_D32_FLOAT;
	DSviewDesc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSviewDesc.Texture2D.MipSlice            = 0u;
	THROW_FAILED_GFX( pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &DSviewDesc, &pDepthStencilView ) );

	// Set render target view
	pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get() );

	// Setup Viewport
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX       = (FLOAT)clientRect.left;
	vp.TopLeftY       = (FLOAT)clientRect.top;
	vp.Width          = (FLOAT)clientRect.right;
	vp.Height         = (FLOAT)clientRect.bottom;
	vp.MinDepth       = 0;
	vp.MaxDepth       = 1;
	pContext->RSSetViewports( 1u, &vp );
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame()
{
	// Handle ImGui
	if ( enableImGui )
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	float c[ 4 ] = { 0.f,0.0f,0.f,1.f };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(), c);
	pContext->ClearDepthStencilView( pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void Graphics::Draw( UINT vertexCount, UINT start )
{
	pContext->Draw( vertexCount, start );
}

void Graphics::DrawIndexed( UINT indexCount )
{
	pContext->DrawIndexed( indexCount, 0u, 0u );
}

void Graphics::EndFrame()
{
	// Handle ImGui
	if ( enableImGui )
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		ImGui::EndFrame();
	}

	HRESULT hr;

	// Present back buffer
	if ( FAILED( hr = pSwapChain->Present( enableVSync ? 1u : 0u, 0u ) ) )
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

void Graphics::SetProjection( DirectX::FXMMATRIX proj ) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
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
