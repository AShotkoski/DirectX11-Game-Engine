#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"
#include "Colors.h"
#include "Vertex.h"
#include "BindableCodex.h"
#include "ConstantBufferEx.h"

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
	AddBind( Binds::Topology::Resolve( gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	Vert::VertexLayout vLayout;
	vLayout.Append( Vert::VertexLayout::Position_3D );
	vLayout.Append( Vert::VertexLayout::Normal );
	Vert::VertexBuffer vertBuf( std::move( vLayout ) );
	IndexedTriangleList itl = GeometricPrim::Cube::GetIndependentFaces( vertBuf );
	itl.SetNormalsIndependentFlat();
	AddBind( Binds::VertexBuffer::Resolve( gfx, itl.vb, "Cube" ) );
	AddBind( Binds::IndexBuffer::Resolve( gfx, itl.indices, "Cube" ) );
	AddBind( Binds::PixelShader::Resolve( gfx, L"PSPhongSolid.cso" ) );
	AddBind( Binds::VertexShader::Resolve( gfx, L"VSPhongSolid.cso" ) );
	auto vs = QueryBindable<Binds::VertexShader>();
	auto vsbytecode = vs->pGetBytecode();
	AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );
	AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );

	// Setup phong material properties
	CB::Layout cblay;
	cblay.add( CB::Float, "specularInt" );
	cblay.add( CB::Float, "specularPow" );
	cblay.add( CB::Float3, "color" );
	CB::Buffer cbbuf( std::move( cblay ) );
	cbbuf["specularInt"] = 0.4f;
	cbbuf["specularPow"] = 500.f;
	cbbuf["color"] = DirectX::XMFLOAT3{ 1.f,0.5,0.05f };
	AddBind( std::make_shared<Binds::CachingPSConstantBufferEx>( gfx, cbbuf, 1u ) );
}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	using namespace DirectX;
		return XMMatrixScaling( size.x, size.y, size.z )
			* XMMatrixRotationRollPitchYaw(pitch,yaw,roll)
			* XMMatrixTranslation(pos.x,pos.y,pos.z);
}
