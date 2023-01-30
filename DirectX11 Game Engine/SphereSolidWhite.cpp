#include "SphereSolidWhite.h"
#include "BindableBaseIncludes.h"
#include "TransformationConstBuffer.h"
#include "SpherePrimitive.h"

SphereSolidWhite::SphereSolidWhite( Graphics& gfx, float radius )
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
		auto itl = GeometricPrim::Sphere::Make<Vertex>();
		itl.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

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

void SphereSolidWhite::Update( float dt )
{
}

void SphereSolidWhite::SetPos( DirectX::XMFLOAT3 position ) noexcept
{
	pos = position;
}

DirectX::XMMATRIX SphereSolidWhite::GetTransformationMatrix() const noexcept
{
	return DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));
}
