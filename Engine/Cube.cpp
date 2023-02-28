#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"
#include "Colors.h"
#include "Vertex.h"
#include "BindableCodex.h"
#include "ConstantBufferEx.h"
#include "Stencil.h"

Cube::Cube(
	Graphics& gfx,
	DirectX::XMFLOAT3 size,
	DirectX::XMFLOAT3 position,
	float pitch, float yaw, float roll )
	: size( size )
	, pos( position )
	, pitch( pitch )
	, yaw( yaw )
	, roll( roll )
{
	Vert::VertexLayout vLayout;
	vLayout.Append( Vert::VertexLayout::Position_3D );
	vLayout.Append( Vert::VertexLayout::Normal );
	Vert::VertexBuffer vertBuf( std::move( vLayout ) );
	IndexedTriangleList itl = GeometricPrim::Cube::GetIndependentFaces( vertBuf );
	itl.SetNormalsIndependentFlat();
	// Set common idx buf, vert buf, and topology on parent drawable
	pVertexBuffer = Binds::VertexBuffer::Resolve( gfx, itl.vb, "Cube" );
	pIndexBuffer = Binds::IndexBuffer::Resolve( gfx, itl.indices, "Cube" );
	pTopology = Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Create Technique for phong
	Technique solidPhong;
	Step only( 0 );
	// Add binds to step needed to phong
	only.AddBind( Binds::PixelShader::Resolve( gfx, L"PSPhongSolid.cso" ) );
	auto pVS = Binds::VertexShader::Resolve( gfx, L"VSPhongSolid.cso" );
	auto vsbytecode = pVS->pGetBytecode();
	only.AddBind( std::move( pVS ) );
	only.AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
	only.AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );
	// Setup phong material properties
	CB::Layout cblay;
	cblay.add( CB::Float, "specularInt" );
	cblay.add( CB::Float, "specularPow" );
	cblay.add( CB::Float3, "color" );
	CB::Buffer cbbuf( std::move( cblay ) );
	cbbuf["specularInt"] = 0.4f;
	cbbuf["specularPow"] = 500.f;
	cbbuf["color"] = DirectX::XMFLOAT3{ 1.f,0.5,0.05f };
	only.AddBind( std::make_shared<Binds::CachingPSConstantBufferEx>( gfx, cbbuf, 1u ) );
	solidPhong.AddStep( std::move( only ) );
	/*
	// Outline
	//itl.Transform( DirectX::XMMatrixScaling( 2, 2, 2 ) );
	outlineBinds.push_back( Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	outlineBinds.push_back( Binds::VertexBuffer::Resolve( gfx, itl.vb, "Cube"));
	outlineBinds.push_back( Binds::IndexBuffer::Resolve( gfx, itl.indices, "Cube"));
	outlineBinds.push_back( Binds::PixelShader::Resolve( gfx, L"PSSolid.cso" ));
	outlineBinds.push_back( Binds::VertexShader::Resolve( gfx, L"VSTransform.cso"));
	auto vss = dynamic_cast<Binds::VertexShader*>(outlineBinds.back().get());
	vsbytecode = vss->pGetBytecode();
	outlineBinds.push_back( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
	outlineBinds.push_back( Binds::TransformationConstBuffer::Resolve( gfx, *this ));
	outlineBinds.push_back( std::make_shared<Binds::Stencil>( gfx, Binds::Stencil::Mode::Mask ) );
	struct CBb
	{
		float r = 1.f;
		float g = 0.f;
		float b = 0.f;
		float pad;
	} cbb;
	outlineBinds.push_back( Binds::PixelConstantBuffer<CBb>::Resolve( gfx, cbb, "solidred", 1u ) );
	*/
}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	using namespace DirectX;
	auto scale = XMMatrixScaling( size.x, size.y, size.z );
	if ( outlining )
		scale = scale * XMMatrixScaling( 1.1f, 1.1f, 1.1f );
		return scale
			* XMMatrixRotationRollPitchYaw(pitch,yaw,roll)
			* XMMatrixTranslation(pos.x,pos.y,pos.z);
}

void Cube::DrawOutline( Graphics& gfx )
{
	/*
	outlining = true;
	for ( auto& b : outlineBinds )
	{
		b->Bind( gfx );
	}
	gfx.DrawIndexed( QueryBindable<Binds::IndexBuffer>()->GetIndicesCount() );
	outlining = false;
	*/
}
