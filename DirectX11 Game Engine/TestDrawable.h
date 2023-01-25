#pragma once

#include "Graphics.h"
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include <DirectXMath.h>

class tDrawable : public Drawable
{
public:
	tDrawable(Graphics& gfx)
	{
		// Set topology
		AddBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

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
					 {  DirectX::XMVectorSet( 0.01f,0.5f,  1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
					 {  DirectX::XMVectorSet( 0.25f,0.0f,  1.0f,1.0f ), 1.0f, 0.0f, 0.0f },
					 {  DirectX::XMVectorSet( -0.25f,0.0f,  1.0f,1.0f ), 0.0f, 1.0f, 0.0f }
		};
		//{
			//{DirectX::XMVectorSet( 0.25f,0.5f,  1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			//{DirectX::XMVectorSet( 0.5f,0.25f,  1.0f,1.0f ), 0.0f, 1.0f, 0.0f },
			//{DirectX::XMVectorSet( 0.5f,-0.25f, 1.0f,1.0f ), 0.0f, 0.5f, 0.0f },
			//{DirectX::XMVectorSet( 0.25f,-0.5f, 1.0f,1.0f ), 0.7f, 0.5f, 0.0f },
			//{DirectX::XMVectorSet( -0.25f,-0.5f,1.0f,1.0f ), 1.0f, 0.0f, 0.0f },
			//{DirectX::XMVectorSet( -0.5f,-0.25f,1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			//{DirectX::XMVectorSet( -0.5f,0.25f, 1.0f,1.0f ), 1.0f, 0.0f, 1.0f },
			//{DirectX::XMVectorSet( -0.25f,0.5f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			//{DirectX::XMVectorSet( 0.0f,0.8f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			//{DirectX::XMVectorSet( 0.5f,0.0f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
			////{DirectX::XMVectorSet( -0.25f,0.0f, 1.0f,1.0f ), 0.0f, 0.0f, 1.0f },
		//};

		
		// Bind vert buff
		AddBind( std::make_unique<VertexBuffer>( gfx, verts) );

		// Bind PS
		AddBind( std::make_unique<PixelShader>( gfx, L"PixelShader.cso"));
		
		// Bind VS, store bytecode
		auto vs = std::make_unique<VertexShader>( gfx, L"VertexShader.cso");
		auto vsbtyecode = vs->pGetBytecode();
		AddBind( std::move( vs ) );

		// INput layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
		IED.emplace_back(  "Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0  );
		IED.emplace_back( "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 );

		AddBind( std::make_unique<InputLayout>( gfx, std::move(IED), *vsbtyecode ) );

	}
	void Update( float dt ) override
	{
		
	}
};