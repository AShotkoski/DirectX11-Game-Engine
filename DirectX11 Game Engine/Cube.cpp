#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"
#include "Colors.h"
#include "Vertex.h"
#include "BindableCodex.h"
#include <random>

Cube::Cube(
	Graphics& gfx,
	DirectX::XMFLOAT3 size,
	DirectX::XMFLOAT3 position,
	float pitch, float yaw, float roll,
	Material material )
	: size(size)
	, pos(position)
	, mat(material)
	, pitch(pitch)
	, yaw(yaw)
	, roll(roll)
{
		// Set topology
		AddBind( Binds::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );
		// Set vertexs
		Vert::VertexLayout vLayout;
		vLayout.Append( Vert::VertexLayout::Position_3D );
		vLayout.Append( Vert::VertexLayout::Normal );

		Vert::VertexBuffer vertBuf( std::move(vLayout) );

		IndexedTriangleList itl = GeometricPrim::Cube::GetIndependentFaces(vertBuf);

		itl.SetNormalsIndependentFlat();

		// Bind vertex buffer
		AddBind( Binds::VertexBuffer::Resolve( gfx, itl.vb, "Cube" ) );
		// Bind Index Buffer
		AddBind( Binds::IndexBuffer::Resolve( gfx, itl.indices, "Cube" ) );
		// Bind PS
		AddBind( Binds::PixelShader::Resolve( gfx, L"PSPhongSolidC.cso" ) );
		// Bind VS, store bytecode
		AddBind( Binds::VertexShader::Resolve( gfx, L"VSPhongSolidC.cso" ) );
		auto vs = QueryBindable<Binds::VertexShader>();
		assert( vs );
		auto vsbytecode = vs->pGetBytecode();

		AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );

		AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );

		// Setup phong material properties
		AddBind( Binds::PixelConstantBuffer<Material>::Resolve( gfx, mat, mat.GetUID(), 1u));
}

void Cube::Update( float dt )
{}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	using namespace DirectX;
		return XMMatrixScaling( size.x, size.y, size.z )
			* XMMatrixRotationRollPitchYaw(pitch,yaw,roll)
			* XMMatrixTranslation(pos.x,pos.y,pos.z);
}
