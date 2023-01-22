#include "Graphics.h"
#include "Macros.h"
#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace WRL = Microsoft::WRL;

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
	WRL::ComPtr<ID3D11Resource> pBackBuffer;

	THROW_FAILED_GFX(pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Resource ), &pBackBuffer  ));
	
	THROW_FAILED_GFX(pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pRenderTargetView));

}

void Graphics::BeginFrame()
{
	float c[ 4 ] = { 1.f,0.5f,0.f,1.f };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(), c);
}

void Graphics::DrawTest()
{
	HRESULT hr;
	// Vertex desc
	struct Vertex
	{
		float x;
		float y;
	};
	// Set vertexs
	Vertex verts[] =
	{
		{ 0.f, 0.5f },
		{ 0.5f, 0.f },
		{ 0.0f,0.0f }
	};

	// Bind vertex buffer
	D3D11_SUBRESOURCE_DATA srd = { 0 };
	srd.pSysMem = verts;
	srd.SysMemPitch = 0u; // Texture
	srd.SysMemSlicePitch = 0u;
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof( bd );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof( Vertex );
	WRL::ComPtr<ID3D11Buffer> pVertBuffer;
	THROW_FAILED_GFX( pDevice->CreateBuffer( &bd, &srd, &pVertBuffer ) );
	const UINT stride = sizeof( Vertex );
	const UINT offset = 0u;
	pContext->IASetVertexBuffers( 0u, 1u, pVertBuffer.GetAddressOf(), &stride, &offset );

	// Set primitive topology
	pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Pixel Shader
	WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	WRL::ComPtr<ID3DBlob> pBlob;
	THROW_FAILED_GFX( D3DReadFileToBlob( L"PixelShader.cso", &pBlob ) );
	THROW_FAILED_GFX( pDevice->CreatePixelShader( pBlob->GetBufferPointer(),
												  pBlob->GetBufferSize(),
												  nullptr, &pPixelShader ) );
	pContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );

	// Create Vertex Shader
	WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	THROW_FAILED_GFX(D3DReadFileToBlob( L"VertexShader.cso", &pBlob ));
	THROW_FAILED_GFX( pDevice->CreateVertexShader(pBlob->GetBufferPointer(),
												   pBlob->GetBufferSize(),
												   nullptr, &pVertexShader) );
	pContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );


	// Set input layout
	WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	D3D11_INPUT_ELEMENT_DESC IED[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	THROW_FAILED_GFX(pDevice->CreateInputLayout( IED, 1, pBlob->GetBufferPointer(),
								pBlob->GetBufferSize(), &pInputLayout ));
	pContext->IASetInputLayout( pInputLayout.Get() );

	// Set render target view
	pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), nullptr );

	// Viewport
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	pContext->RSSetViewports( 1u, &vp );


	

	pContext->Draw( (UINT)std::size(verts), 0);
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
