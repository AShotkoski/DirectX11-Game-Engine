#include "SphereSolidWhite.h"
#include "Binds/BindableBaseIncludes.h"
#include "SpherePrimitive.h"
#include "Graphics/Vertex.h"

SphereSolidWhite::SphereSolidWhite( Graphics& gfx, float radius, RDG::RenderGraph* pGraph )
{
	// Set parent binds
	pTopology =  Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) ;
	Vert::VertexLayout vl;
	vl.Append( Vert::VertexLayout::Position_3D );
	Vert::VertexBuffer vb( std::move( vl ) );
	auto itl = GeometricPrim::Sphere::MakeTesselated( 12, 8, vb );
	itl.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );
	pVertexBuffer = Binds::VertexBuffer::Resolve( gfx, itl.vb, "Light");
	pIndexBuffer =  Binds::IndexBuffer::Resolve( gfx, itl.indices, "lightidx");

	Technique Solid;
	Step only( "lambertian" );
	only.LinkTarget( *pGraph );

	only.AddBind( Binds::PixelShader::Resolve( gfx, L"PSSolid.cso"));
	auto pVS =  Binds::VertexShader::Resolve( gfx, L"VSTransform.cso" ) ;
	auto vsbytecode = pVS->pGetBytecode();
	only.AddBind( std::move( pVS ) );
	only.AddBind( Binds::InputLayout::Resolve( gfx, vb.GetLayout(), *vsbytecode ) );
	only.AddBind( std::make_shared<Binds::TransformationConstBuffer>( gfx ) );

	// make white
	struct CB
	{
		float r = 1.f;
		float g = 1.f;
		float b = 1.f;
		float pad = 0;
	} cb;
	only.AddBind( Binds::PixelConstantBuffer<CB>::Resolve( gfx, cb, "solidwhite", 1u ) );
	Solid.AddStep( std::move( only ) );
	AddTechnique( std::move( Solid ) );
}

void SphereSolidWhite::SetPos( DirectX::XMFLOAT3 position ) noexcept
{
	pos = position;
}

DirectX::XMMATRIX SphereSolidWhite::GetTransformationMatrix() const noexcept
{
	return DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));
}
