#include "SphereSolidWhite.h"
#include "BindableBaseIncludes.h"
#include "TransformationConstBuffer.h"
#include "SpherePrimitive.h"
#include "Vertex.h"

SphereSolidWhite::SphereSolidWhite( Graphics& gfx, float radius )
{
	// Set topology
	AddBind( Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	// Set vertexs
	Vert::VertexLayout vl;
	vl.Append( Vert::VertexLayout::Position_3D );
	Vert::VertexBuffer vb( std::move( vl ) );

	auto itl = GeometricPrim::Sphere::MakeTesselated( 12, 8, vb );

	itl.Transform( DirectX::XMMatrixScaling( radius, radius, radius ) );

	// Bind vertex buffer
	AddBind( Binds::VertexBuffer::Resolve( gfx, itl.vb, "Light"));
	// Bind Index Buffer
	AddBind( Binds::IndexBuffer::Resolve( gfx, itl.indices, "lightidx"));
	// Bind PS
	AddBind( Binds::PixelShader::Resolve( gfx, L"PSSolid.cso"));
	// Bind VS, store bytecode
	AddBind( Binds::VertexShader::Resolve( gfx, L"VSTransform.cso" ) );
	auto vs = QueryBindable<Binds::VertexShader>();
	assert( vs );
	auto vsbytecode = vs->pGetBytecode();

	AddBind( Binds::InputLayout::Resolve( gfx, vb.GetLayout(), *vsbytecode ) );

	// Bind non static Transformation CB
	AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );
	// make white
	struct CB
	{
		float r = 1.f;
		float g = 1.f;
		float b = 1.f;
		float pad;
	} cb;
	AddBind( Binds::PixelConstantBuffer<CB>::Resolve( gfx, cb, "solidwhite", 1u ) );
}

void SphereSolidWhite::SetPos( DirectX::XMFLOAT3 position ) noexcept
{
	pos = position;
}

DirectX::XMMATRIX SphereSolidWhite::GetTransformationMatrix() const noexcept
{
	return DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));
}
