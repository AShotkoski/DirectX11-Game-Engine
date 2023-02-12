#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"
#include "Colors.h"
#include "Vertex.h"
#include "BindableCodex.h"
#include <random>

Cube::Cube( Graphics& gfx, float size, float rho, float theta, float phi, 
			DirectX::XMFLOAT3 matColor, float specInt, float specPow )
	:
	size(size),
	rho(rho),
	theta(theta),
	phi(phi)
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
		AddBind( Binds::VertexBuffer::Resolve( gfx, itl.vb, "SUPER COOL TEMP" ) );
		// Bind Index Buffer
		AddBind( Binds::IndexBuffer::Resolve( gfx, itl.indices, "SUPER COOL TEMP idx" ) );
		// Bind PS
		AddBind( Binds::PixelShader::Resolve( gfx, L"PSPhong.cso" ) );
		// Bind VS, store bytecode
		AddBind( Binds::VertexShader::Resolve( gfx, L"VSPhong.cso" ) );
		auto vs = QueryBindable<Binds::VertexShader>();
		assert( vs );
		auto vsbytecode = vs->pGetBytecode();

		AddBind( Binds::InputLayout::Resolve( gfx, vertBuf.GetLayout(), *vsbytecode ) );

		AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );

		// Setup phong material properties
		struct PSCBuf
		{
			DirectX::XMFLOAT3 MatCol;
			float specularIntensity;
			alignas( 16 ) float specularPower;
		};

		PSCBuf cubeProps = { matColor, specInt, specPow };

		AddBind( Binds::PixelConstantBuffer<PSCBuf>::Resolve( gfx, cubeProps,"broke", 1u));
}

Cube::Cube( Graphics& gfx, float size, float rho, float theta, float phi, 
			 float dTheta, float dPhi, float dPitch, float dYaw, float dRoll, DirectX::XMFLOAT3 matColor
			, float specInt, float specPow )
	:
	Cube(gfx,size,rho,theta,phi,matColor, specInt, specPow)
{
	this->dTheta = dTheta;
	this->dPhi = dPhi;
	this->dPitch = dPitch;
	this->dYaw = dYaw;
	this->dRoll = dRoll;
}

void Cube::Update( float dt )
{
	pitch += dPitch * dt;
	yaw += dYaw * dt;
	roll += dRoll * dt;

	theta += dTheta * dt;
	phi += dPhi * dt;
}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	if ( sizeX == -1 || sizeY == -1 || sizeZ == -1 )
	{

		return DirectX::XMMatrixScaling( size, size, size )
			* DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll )
			* DirectX::XMMatrixTranslation( 0.f, 0.f, rho )
			* DirectX::XMMatrixRotationRollPitchYaw( phi, theta, 0 );
	}
	else
	{
		return DirectX::XMMatrixScaling( sizeX, sizeY, sizeZ )
			* DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll )
			* DirectX::XMMatrixTranslation( 0.f, 0.f, rho )
			* DirectX::XMMatrixRotationRollPitchYaw( phi, theta, 0 );
	}
}
