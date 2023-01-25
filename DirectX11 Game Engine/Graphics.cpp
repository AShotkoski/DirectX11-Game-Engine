#include "Graphics.h"
#include "Macros.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace WRL = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics( HWND hWnd )
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

	// Create render target view
	WRL::ComPtr<ID3D11Resource> pBackBuffer;

	THROW_FAILED_GFX(pSwapChain->GetBuffer( 0u, __uuidof( ID3D11Resource ), &pBackBuffer  ));
	
	THROW_FAILED_GFX(pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pRenderTargetView));

	// Setup Viewport
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX       = (FLOAT)clientRect.left;
	vp.TopLeftY       = (FLOAT)clientRect.top;
	vp.Width          = (FLOAT)clientRect.right;
	vp.Height         = (FLOAT)clientRect.bottom;
	vp.MinDepth       = 0;
	vp.MaxDepth       = 1;
	pContext->RSSetViewports( 1u, &vp );

	// Set render target view
	pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), nullptr );
}

void Graphics::BeginFrame()
{
	float c[ 4 ] = { 1.f,0.5f,0.f,1.f };
	pContext->ClearRenderTargetView( pRenderTargetView.Get(), c);
}

void Graphics::DrawTest(float angle)
{
	HRESULT hr;
	// Vertex desc
	struct Vertex
	{
		dx::XMVECTOR pos;

		float r;
		float g;
		float b;
	};
	// Set vertexs
	Vertex verts[] =
	{
		{dx::XMVectorSet(0.25f,0.5f,  1.0f,1.0f), 1.0f, 0.0f, 1.0f },
		{dx::XMVectorSet(0.5f,0.25f,  1.0f,1.0f), 0.0f, 1.0f, 0.0f },
		{dx::XMVectorSet(0.5f,-0.25f, 1.0f,1.0f), 0.0f, 0.5f, 0.0f },
		{dx::XMVectorSet(0.25f,-0.5f, 1.0f,1.0f), 0.7f, 0.5f, 0.0f },
		{dx::XMVectorSet(-0.25f,-0.5f,1.0f,1.0f), 1.0f, 0.0f, 0.0f },
		{dx::XMVectorSet(-0.5f,-0.25f,1.0f,1.0f), 0.0f, 0.0f, 1.0f },
		{dx::XMVectorSet(-0.5f,0.25f, 1.0f,1.0f), 1.0f, 0.0f, 1.0f },
		{dx::XMVectorSet(-0.25f,0.5f, 1.0f,1.0f), 0.0f, 0.0f, 1.0f },
	};


	// Bind vertex buffer
	WRL::ComPtr<ID3D11Buffer> pVertBuffer;
	D3D11_SUBRESOURCE_DATA srd = { 0 };
	srd.pSysMem = verts;
	srd.SysMemPitch = 0u; // Texture
	srd.SysMemSlicePitch = 0u;
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof( verts );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof( Vertex );
	THROW_FAILED_GFX( pDevice->CreateBuffer( &bd, &srd, &pVertBuffer ) );
	const UINT stride = sizeof( Vertex );
	const UINT offset = 0u;
	pContext->IASetVertexBuffers( 0u, 1u, pVertBuffer.GetAddressOf(), &stride, &offset );

	// Create Index Buffer
	const unsigned short indices[] =
	{
		6,4,5,
		7,4,6,
		7,3,4,
		7,0,3,
		0,2,3,
		0,1,2
	};

	WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof( indices );
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof( indices[0]);
	D3D11_SUBRESOURCE_DATA isrd = { 0 };
	isrd.pSysMem = indices;
	isrd.SysMemPitch = 0u; // Texture
	isrd.SysMemSlicePitch = 0u;
	THROW_FAILED_GFX(pDevice->CreateBuffer(&ibd,&isrd,&pIndexBuffer));
	pContext->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );

	// Create constant buffer
	struct ConstBuffer
	{
		dx::XMMATRIX transformation;
	};

	const ConstBuffer cb =
	{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ( angle )
			* dx::XMMatrixPerspectiveFovLH( 10.f,3.f / 4.f,0.25f,25.0f )
		)
	};

	WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.ByteWidth = sizeof( cb );
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csrd = { 0 };
	csrd.pSysMem = &cb;
	csrd.SysMemPitch = 0u; // Texture
	csrd.SysMemSlicePitch = 0u;
	THROW_FAILED_GFX( pDevice->CreateBuffer( &cbd, &csrd, &pConstBuffer ) );
	pContext->VSSetConstantBuffers( 0u, 1u, pConstBuffer.GetAddressOf() );

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
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	THROW_FAILED_GFX(pDevice->CreateInputLayout( IED, 2, pBlob->GetBufferPointer(),
								pBlob->GetBufferSize(), &pInputLayout ));
	pContext->IASetInputLayout( pInputLayout.Get() );

	

	pContext->DrawIndexed( (UINT)std::size(indices), 0u, 0u);
}

void Graphics::Draw( UINT vertexCount, UINT start )
{
	pContext->Draw( vertexCount, start );
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