#include "Graphics.h"
#include "Macros.h"
#include "../ThirdParty/ImGui/imgui_impl_dx11.h"
#include "../ThirdParty/ImGui/imgui_impl_win32.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11sdklayers.h>
#include <Binds/DepthStencil.h>
#include <Binds/RenderTarget.h>

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
		throw std::runtime_error( "Error getting client rect.\n" ); // todo graphics error
	Width = clientRect.right;
	Height = clientRect.bottom;


	// Setup SwapChain parameters
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width                   = Width;
	sd.BufferDesc.Height                  = Height;
	sd.BufferDesc.RefreshRate.Numerator   = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count                   = 1;
	sd.SampleDesc.Quality                 = 0;
	sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount                        = 2;
	sd.OutputWindow                       = hWnd;
	sd.Windowed                           = true;
	sd.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
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
	if constexpr ( globals::enableImGui )
		ImGui_ImplDX11_Init( pDevice.Get(), pContext.Get() );

	// Get back buffer tex
	WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	THROW_FAILED_GFX(pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Texture2D ), &pBackBuffer  ));
	pRenderTarget = std::dynamic_pointer_cast<RenderTarget>(
		std::make_shared<ExclusiveRenderTarget>( *this, pBackBuffer.Get() ) );
}

Graphics::~Graphics()
{
	if constexpr ( globals::enableImGui )
		ImGui_ImplDX11_Shutdown();
}

void Graphics::BeginFrame()
{
	// Handle ImGui
	if constexpr ( globals::enableImGui )
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void Graphics::Draw( UINT vertexCount, UINT start )
{
	pContext->Draw( vertexCount, start );
}

void Graphics::DrawIndexed( UINT indexCount )
{
	pContext->DrawIndexed( indexCount, 0u, 0u );
}

std::shared_ptr<RenderTarget> Graphics::pGetRenderTarget()
{
	return pRenderTarget;
}

void Graphics::EndFrame()
{
	// Handle ImGui
	if constexpr( globals::enableImGui )
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

UINT Graphics::GetWidth() const
{
	return Width;
}

UINT Graphics::GetHeight() const
{
	return Height;
}

void Graphics::SetProjection( DirectX::FXMMATRIX proj ) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

Camera& Graphics::GetCamera() noexcept
{
	return cam;
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
		<< "[Code] 0x" << std::hex << GetErrorCode() << std::dec << '(' << GetErrorCode() << ')' << std::endl
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
