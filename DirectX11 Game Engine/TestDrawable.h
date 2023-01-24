#pragma once

#include "Drawable.h"
#include <DirectXMath.h>

class tDrawable : public Drawable
{
public:
	tDrawable()
	{
		
	}
	void Update( float dt ) override
	{
		HRESULT hr;
	// Vertex desc
		struct Vertex
		{
			DirectX::XMVECTOR pos;

			float r;
			float g;
			float b;
		};
		// Set vertexs
		Vertex verts[] =
		{
			{dx::XMVectorSet( 0.25f,0.5f,  1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			{dx::XMVectorSet( 0.5f,0.25f,  1.0f,1.0f ), 0.0f, 1.0f, 0.0f },
			{dx::XMVectorSet( 0.5f,-0.25f, 1.0f,1.0f ), 0.0f, 0.5f, 0.0f },
			{dx::XMVectorSet( 0.25f,-0.5f, 1.0f,1.0f ), 0.7f, 0.5f, 0.0f },
			{dx::XMVectorSet( -0.25f,-0.5f,1.0f,1.0f ), 1.0f, 0.0f, 0.0f },
			{dx::XMVectorSet( -0.5f,-0.25f,1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			{dx::XMVectorSet( -0.5f,0.25f, 1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			{dx::XMVectorSet( -0.25f,0.5f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
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
		ibd.StructureByteStride = sizeof( indices[ 0 ] );
		D3D11_SUBRESOURCE_DATA isrd = { 0 };
		isrd.pSysMem = indices;
		isrd.SysMemPitch = 0u; // Texture
		isrd.SysMemSlicePitch = 0u;
		THROW_FAILED_GFX( pDevice->CreateBuffer( &ibd, &isrd, &pIndexBuffer ) );
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
		THROW_FAILED_GFX( D3DReadFileToBlob( L"VertexShader.cso", &pBlob ) );
		THROW_FAILED_GFX( pDevice->CreateVertexShader( pBlob->GetBufferPointer(),
													   pBlob->GetBufferSize(),
													   nullptr, &pVertexShader ) );
		pContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );


		// Set input layout
		WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		D3D11_INPUT_ELEMENT_DESC IED[] =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		THROW_FAILED_GFX( pDevice->CreateInputLayout( IED, 2, pBlob->GetBufferPointer(),
													  pBlob->GetBufferSize(), &pInputLayout ) );
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


		pContext->DrawIndexed( (UINT)std::size( indices ), 0u, 0u );
	}
};