#include "FlappingPlane.h"
#include "BindableBaseIncludes.h"
#include "PlanePrimitive.h"
#include "TransformationConstBuffer.h"

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
		};
	
		// Set vertexs
		auto itl = GeometricPrim::Plane::GetPlain<Vertex>(4);

		itl.Transform( DirectX::XMMatrixRotationX( 1.f ) );

		// Bind vertex buffer
		AddStaticBind( std::make_unique<VertexBuffer>( gfx, itl.vertices ) );

		// Bind Index Buffer
		AddStaticBind( std::make_unique<IndexBuffer>( gfx, itl.indices ) );

		// Bind PS
		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PSSolidWhite.cso" ) );

		// Bind VS, store bytecode
		auto vs = std::make_unique<VertexShader>( gfx, L"VSTransform.cso" );
		auto vsbtyecode = vs->pGetBytecode();
		AddStaticBind( std::move( vs ) );

		// Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
		IED.emplace_back( "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );

		AddStaticBind( std::make_unique<InputLayout>( gfx, std::move( IED ), *vsbtyecode ) );
	}

	// Bind non static Transformation CB
	AddBind( std::make_unique<TransformationConstBuffer>( gfx, *this ) );
}

void FlappingPlane::Update( float dt )
{
}

DirectX::XMMATRIX FlappingPlane::GetTransformationMatrix() const noexcept
{
    return DirectX::XMMatrixTranslation(0,0,5.f);
}
