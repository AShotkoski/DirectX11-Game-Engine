#include "Cube.h"
#include "BindableBaseIncludes.h"
#include "ConstantBuffers.h"
#include "TransformationConstBuffer.h"
#include "CubePrimitive.h"

Cube::Cube( Graphics& gfx,float size, float rho, float theta, float phi )
	:
	size(size),
	rho(rho),
	theta(theta),
	phi(phi)
{
	if ( !isStaticInitialized() )
	{
		// Set topology
		AddStaticBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		// Setup Vertex type
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			struct
			{
				float r;
				float g;
				float b;
			} color;
		};

		// Set vertexs
		auto itl = GeometricPrim::Cube::GetPlain<Vertex>();
		itl.vertices[0].color = { 1.0, 0.f, 0.f };
		itl.vertices[1].color = { 1.0, 1.f, 0.f };
		itl.vertices[2].color = { 0.0, 1.f, 0.f };
		itl.vertices[3].color = { 1.0, 0.f, 1.f };
		itl.vertices[4].color = { 1.0, 0.f, 0.f };
		itl.vertices[5].color = { 0.0, 0.f, 1.f };
		itl.vertices[6].color = { 1.0, 0.f, 0.f };
		itl.vertices[7].color = { 1.0, 1.f, 1.f };

		// Bind vertex buffer
		AddStaticBind( std::make_unique<VertexBuffer>( gfx, itl.vertices ) );

		// Bind Index Buffer
		AddStaticBind( std::make_unique<IndexBuffer>( gfx, itl.indices ) );

		// Bind PS
		AddStaticBind( std::make_unique<PixelShader>( gfx, L"PixelShader.cso" ) );

		// Bind VS, store bytecode
		auto vs = std::make_unique<VertexShader>( gfx, L"VertexShader.cso" );
		auto vsbtyecode = vs->pGetBytecode();
		AddStaticBind( std::move( vs ) );

		// Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> IED;
		IED.emplace_back( "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 );
		IED.emplace_back( "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 );

		AddStaticBind( std::make_unique<InputLayout>( gfx, std::move( IED ), *vsbtyecode ) );
	}

	// Bind non static Transformation CB
	AddBind( std::make_unique<TransformationConstBuffer>( gfx, *this ) );
}

Cube::Cube( Graphics& gfx, float size, float rho, float theta, float phi, 
			 float dTheta, float dPhi, float dPitch, float dYaw, float dRoll )
	:
	Cube(gfx,size,rho,theta,phi)
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
	// Use psuedo spherical coordinates to set position since it's funny
	// First rotate object, then move based on rho theta phi, finally move out 10 units along Z
	return DirectX::XMMatrixScaling(size,size,size)
		* DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll )
		* DirectX::XMMatrixTranslation( rho, 0.f, 0.f )
		* DirectX::XMMatrixRotationRollPitchYaw( 0.f, theta, phi )
		* DirectX::XMMatrixTranslation( 0, 0, 10.f );
}
