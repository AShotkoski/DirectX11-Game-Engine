#pragma once

#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"

class Ray : public DrawableBase<Ray>
{
public:
	Ray(Graphics& gfx)
	{
		namespace dx = DirectX;
		if ( !isStaticInitialized() )
		{
			AddStaticBind( std::make_unique<Binds::Topology>( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ));
			
			Vert::VertexLayout vl;
			vl.Append( Vert::VertexLayout::Position_3D );

			Vert::VertexBuffer vb( std::move( vl ) );
			
			// ----- Vertices ------
			vb.Emplace_back( dx::XMFLOAT3( 0, 0, 0 ) );
			vb.Emplace_back( dx::XMFLOAT3( 0, 10, -10 ) );		
			AddStaticBind( std::make_unique<Binds::VertexBuffer>(gfx, vb) );

			const std::vector< unsigned short> inds =
			{
				0,1
			};
			AddStaticBind( std::make_unique<Binds::IndexBuffer>(gfx, inds ));

			auto vs = std::make_unique<Binds::VertexShader>( gfx, L"VSTransform.cso" );
			auto vsbtyecode = vs->pGetBytecode();
			AddStaticBind( std::move( vs ) );

			AddStaticBind( std::make_unique<Binds::InputLayout>(gfx, vb.GetD3DInputLayout(), *vsbtyecode) );

			AddStaticBind( std::make_unique<Binds::PixelShader>( gfx, L"PSSolidWhite.cso" ));
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
private:

};