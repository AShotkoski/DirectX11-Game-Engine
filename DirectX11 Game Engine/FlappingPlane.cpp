#include "FlappingPlane.h"
#include "BindableBaseIncludes.h"
#include "PlanePrimitive.h"
#include "TransformationConstBuffer.h"
#include "PlatonicSolids.h"
#include "Colors.h"
#include "Texture.h"
#include "Sampler.h"

FlappingPlane::FlappingPlane( Graphics& gfx )
{
	if ( !isStaticInitialized() )
	{
		// Set topology
		AddStaticBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		// Setup Vertex type
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			struct
			{
				float u;
				float v;
			} tc;
		};
	
		// Set vertexs
		auto itl = GeometricPrim::Plane::GetPlain<Vertex>();
		itl.vertices[0].tc = { 0,0 };
		itl.vertices[1].tc = { 1,0 };
		itl.vertices[2].tc = { 0,1 };
		itl.vertices[3].tc = { 1,1 };
		
		//---------tex
		AddStaticBind( std::make_unique<Texture>( gfx, L"Textures\\test.dds" ) );
		AddStaticBind( std::make_unique<Sampler>( gfx ) );

		// Bind vertex buffer
		AddStaticBind( std::make_unique<VertexBuffer>( gfx, itl.vertices ) );

		// Bind Index Buffer
		AddStaticBind( std::make_unique<IndexBuffer>( gfx, itl.indices ) );

		// Bind PS
		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PSTexture.cso" ) );

		// Bind VS, store bytecode
		auto vs = std::make_unique<VertexShader>( gfx, L"VSTexture.cso" );
		auto vsbtyecode = vs->pGetBytecode();
		AddStaticBind( std::move( vs ) );

		// Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
		IED.emplace_back( "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
		IED.emplace_back( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 );

		AddStaticBind( std::make_unique<InputLayout>( gfx, std::move( IED ), *vsbtyecode ) );
		


	}

	// Bind non static Transformation CB
	AddBind( std::make_unique<TransformationConstBuffer>( gfx, *this ) );
}

void FlappingPlane::Update( float dt )
{
	theta += dt;
}

DirectX::XMMATRIX FlappingPlane::GetTransformationMatrix() const noexcept
{
	using namespace DirectX;
    return XMMatrixRotationRollPitchYaw(theta, theta * 2, 0) *
		XMMatrixTranslation(0,0,5.f);
}
