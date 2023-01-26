#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"


Cube::Cube( Graphics& gfx,float size, float rho, float theta, float phi )
	:
	size(size),
	rho(rho),
	theta(theta),
	phi(phi)
{
	// Set topology
	AddBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	// Setup Vertex type
	struct Vertex
	{
		struct
		{
			DirectX::XMVECTOR position;
		} pos;
		struct
		{
			float r;
			float g;
			float b;
		} color;
	};

	// Set vertexs
	const float side = size / 2.f;
	std::vector<Vertex> verts =
	{
		{ DirectX::XMVectorSet( -side, -side, -side, 1.0f ), 1.0f, 0.0f, 1.0f },
		{ DirectX::XMVectorSet( side, -side, -side, 1.0f ), 0.0f, 1.0f, 0.0f },
		{ DirectX::XMVectorSet( -side, side, -side, 1.0f ), 0.0f, 0.5f, 0.0f },
		{ DirectX::XMVectorSet( side, side, -side, 1.0f ), 0.7f, 0.5f, 0.0f },
		{ DirectX::XMVectorSet( -side, -side, side, 1.0f ), 1.0f, 0.0f, 0.0f },
		{ DirectX::XMVectorSet( side, -side, side, 1.0f ), 0.0f, 0.0f, 1.0f },
		{ DirectX::XMVectorSet( -side, side, side, 1.0f ), 1.0f, 0.0f, 1.0f },
		{ DirectX::XMVectorSet( side, side, side, 1.f ), 0.0f, 0.0f, 1.0f },
	};

	// Bind vertex buffer
	AddBind( std::make_unique<VertexBuffer>( gfx, verts ) );

	// Create Index Buffer
	const std::vector<unsigned short> indices =
	{
		0,2,1,   2,3,1,
		1,3,5,	 3,7,5,
		5,7,4,	 7,6,4,
		4,6,2,	 2,0,4,
		2,6,3,   6,7,3,
		0,1,4,	 1,5,4
	};

	// Bind Index Buffer
	AddBind( std::make_unique<IndexBuffer>( gfx, indices ) );

	// Bind Transformation CB
	AddBind( std::make_unique<TransformationConstBuffer>( gfx, *this ) );

	// Bind PS
	AddBind( std::make_unique<PixelShader>( gfx, L"PixelShader.cso" ) );

	// Bind VS, store bytecode
	auto vs = std::make_unique<VertexShader>( gfx, L"VertexShader.cso" );
	auto vsbtyecode = vs->pGetBytecode();
	AddBind( std::move( vs ) );

	// Input layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
	IED.emplace_back( "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
	IED.emplace_back( "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 );

	AddBind( std::make_unique<InputLayout>( gfx, std::move( IED ), *vsbtyecode ) );
}

void Cube::Update( float dt )
{
	pitch += dPitch * dt;
	yaw += dYaw * dt;
	roll += dRoll * dt;
}

DirectX::XMMATRIX Cube::GetTransformationMatrix() const noexcept
{
	// Use psuedo spherical coordinates to set position since it's funny
	// First rotate object, then move based on rho theta phi, finally move out 10 units along Z
	return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll )
		* DirectX::XMMatrixTranslation( rho, 0.f, 0.f )
		* DirectX::XMMatrixRotationRollPitchYaw( 0.f, theta, phi )
		* DirectX::XMMatrixTranslation( 0, 0, 10.f );
}
