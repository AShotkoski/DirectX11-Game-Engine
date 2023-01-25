#pragma once

#include "Graphics.h"
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include <DirectXMath.h>

class tDrawable : public Drawable
{
public:
	tDrawable(Graphics& gfx)
	{
		// Set topology
		AddBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		// Setup Vertex type
		struct Vertex
		{
			struct
			{
				DirectX::XMVECTOR position;
			} pos;	
			struct
			{
				float r;
				float g;
				float b;
			} color;
		};

		// Set vertexs
		std::vector<Vertex> verts = 
		{
			{DirectX::XMVectorSet( 0.25f,0.5f,  1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			{DirectX::XMVectorSet( 0.5f,0.25f,  1.0f,1.0f ), 0.0f, 1.0f, 0.0f },
			{DirectX::XMVectorSet( 0.5f,-0.25f, 1.0f,1.0f ), 0.0f, 0.5f, 0.0f },
			{DirectX::XMVectorSet( 0.25f,-0.5f, 1.0f,1.0f ), 0.7f, 0.5f, 0.0f },
			{DirectX::XMVectorSet( -0.25f,-0.5f,1.0f,1.0f ), 1.0f, 0.0f, 0.0f },
			{DirectX::XMVectorSet( -0.5f,-0.25f,1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			{DirectX::XMVectorSet( -0.5f,0.25f, 1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			{DirectX::XMVectorSet( -0.25f,0.5f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
		};

		
		// Bind vertex buffer
		AddBind( std::make_unique<VertexBuffer>( gfx, verts) );

		// Create Index Buffer
		const std::vector<unsigned short> indices =
		{
			6,4,5,
			7,4,6,
			7,3,4,
			7,0,3,
			0,2,3,
			0,1,2
		};

		// Bind Index Buffer
		AddBind( std::make_unique<IndexBuffer>( gfx, indices ) );

		// Bind Transformation CB
		AddBind( std::make_unique<TransformationConstBuffer>( gfx, *this ) );

		// Bind PS
		AddBind( std::make_unique<PixelShader>( gfx, L"PixelShader.cso"));
		
		// Bind VS, store bytecode
		auto vs = std::make_unique<VertexShader>( gfx, L"VertexShader.cso");
		auto vsbtyecode = vs->pGetBytecode();
		AddBind( std::move( vs ) );

		// Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
		IED.emplace_back(  "Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0  );
		IED.emplace_back( "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 );

		AddBind( std::make_unique<InputLayout>( gfx, std::move(IED), *vsbtyecode ) );

	}

	void Update( float dt ) override
	{
		theta += dt;
	}

	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override
	{
		return DirectX::XMMatrixRotationRollPitchYaw( 0.0f, 0.0f, theta );
	}
private:
	float theta = 0.0f;
};