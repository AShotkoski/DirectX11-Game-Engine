#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"
#include "Colors.h"
#include "Vertex.h"
#include <random>

Cube::Cube( Graphics& gfx, float size, float rho, float theta, float phi, 
			DirectX::XMFLOAT3 matColor, float specInt, float specPow )
	:
	size(size),
	rho(rho),
	theta(theta),
	phi(phi)
{
	if ( !isStaticInitialized() )
	{
		// Set topology
		AddStaticBind( std::make_unique<Binds::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		// Set vertexs
		Vert::VertexLayout vLayout;
		vLayout.Append( Vert::VertexLayout::Position_3D );
		vLayout.Append( Vert::VertexLayout::Normal );

		Vert::VertexBuffer vertBuf( std::move(vLayout) );

		IndexedTriangleList itl = GeometricPrim::Cube::GetIndependentFaces(vertBuf);

		itl.SetNormalsIndependentFlat();

		// Bind vertex buffer
		AddStaticBind( std::make_unique<Binds::VertexBuffer>( gfx, itl.vb ) );

		// Bind Index Buffer
		AddStaticBind( std::make_unique<Binds::IndexBuffer>( gfx, itl.indices ) );

		// Bind PS
		AddStaticBind( std::make_unique<Binds::PixelShader>( gfx, L"PSPhong.cso" ) );

		// Bind VS, store bytecode
		auto vs = std::make_unique<Binds::VertexShader>( gfx, L"VSPhong.cso" );
		auto vsbtyecode = vs->pGetBytecode();
		AddStaticBind( std::move( vs ) );

		AddStaticBind( std::make_unique<Binds::InputLayout>( gfx, vertBuf.GetD3DInputLayout(), *vsbtyecode ) );
	}

	// Bind non static Transformation CB
	AddBind( std::make_unique<Binds::TransformationConstBuffer>( gfx, *this ) );

	// Setup phong material properties
	struct PSCBuf
	{
		DirectX::XMFLOAT3 MatCol;
		float specularIntensity;
		alignas( 16 ) float specularPower;
	};

	PSCBuf cubeProps = { matColor, specInt, specPow };

	AddBind( std::make_unique<Binds::PixelConstantBuffer<PSCBuf>>( gfx, cubeProps, 1u ) );
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
