#pragma once

#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"

class Ray : public DrawableBase<Ray>
{
public:
	Ray( Graphics& gfx, DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 dir, float length = 25.f )
	{
		namespace dx = DirectX;
		if ( !isStaticInitialized() )
		{
			AddStaticBind( std::make_unique<Binds::Topology>( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ) );

			Vert::VertexLayout vl;
			vl.Append( Vert::VertexLayout::Position_3D );

			Vert::VertexBuffer vb( std::move( vl ) );

			// ----- Vertices ------
			dx::XMStoreFloat3(
				&dir,
				dx::XMVectorScale(
					dx::XMVector3Normalize( dx::XMLoadFloat3( &dir ) )
					, length 
				)
			);
			vb.Emplace_back( source );
			vb.Emplace_back( dir );

			AddStaticBind( std::make_unique<Binds::VertexBuffer>( gfx, vb ) );

			auto vs = std::make_unique<Binds::VertexShader>( gfx, L"VSTransform.cso" );
			auto vsbtyecode = vs->pGetBytecode();
			AddStaticBind( std::move( vs ) );

			AddStaticBind( std::make_unique<Binds::InputLayout>( gfx, vb.GetD3DInputLayout(), *vsbtyecode ) );

			AddStaticBind( std::make_unique<Binds::PixelShader>( gfx, L"PSSolidWhite.cso" ) );
		}

		AddBind( std::make_unique<Binds::TransformationConstBuffer>( gfx, *this ) );
	}
	virtual void Update( float dt ) override
	{

	}
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override
	{
		return DirectX::XMMatrixIdentity();
	}
	// override draw function to call drawindexed
	virtual void Draw( Graphics& gfx ) const override
	{
		DrawNoIndex( gfx, 2u );
	}
};
/// MOFO ADD JUST A ORIGIN, AND DIRECTION, AND FIGURE IT OUT FROM THERE. SMH, NO NEW SHADERS IF YOU WILL