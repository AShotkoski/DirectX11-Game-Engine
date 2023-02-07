#pragma once

#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"

class Ray : public DrawableBase<Ray>
{
public:
	Ray( Graphics& gfx, DirectX::XMFLOAT3 source, DirectX::XMFLOAT3 dir, float length = 25.f )
		: vertbuf( Vert::VertexLayout{ }.Append( Vert::VertexLayout::Position_3D ))
		, len(length)
	{
		namespace dx = DirectX;
		if ( !isStaticInitialized() )
		{
			AddStaticBind( std::make_unique<Binds::Topology>( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ) );

			// ----- Vertices ------
			vertbuf.Emplace_back( source );
			vertbuf.Emplace_back( dir );
			// Adjust direction
			SetDirHelper();

			auto vb = std::make_unique<Binds::DynamicVertexBuffer>( gfx, vertbuf );
			dynamicVB = vb.get();
			AddStaticBind( std::move(vb) );

			auto vs = std::make_unique<Binds::VertexShader>( gfx, L"VSTransform.cso" );
			auto vsbtyecode = vs->pGetBytecode();
			AddStaticBind( std::move( vs ) );

			AddStaticBind( std::make_unique<Binds::InputLayout>( gfx, vertbuf.GetD3DInputLayout(), *vsbtyecode ) );

			AddStaticBind( std::make_unique<Binds::PixelShader>( gfx, L"PSSolidWhite.cso" ) );
		}

		AddBind( std::make_unique<Binds::TransformationConstBuffer>( gfx, *this ) );
	}
	void SetDir(Graphics& gfx, DirectX::XMFLOAT3 dir)
	{	
		vertbuf[1].Attribute<Vert::VertexLayout::Position_3D>() = dir;
		SetDirHelper();
		dynamicVB->Update( gfx, vertbuf );
	}
	void SetOrigin(Graphics& gfx, DirectX::XMFLOAT3 origin)
	{
		vertbuf[0].Attribute<Vert::VertexLayout::Position_3D>() = origin;
		dynamicVB->Update( gfx, vertbuf );
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
private:
	// Lazy helper class to set direction vector 
	void SetDirHelper()
	{
		namespace dx = DirectX;
		const auto src = dx::XMLoadFloat3( &vertbuf[0].Attribute<Vert::VertexLayout::Position_3D>() );
		auto dir = dx::XMLoadFloat3( &vertbuf[1].Attribute<Vert::VertexLayout::Position_3D>() );

		dir = dx::XMVectorScale( dx::XMVector3Normalize( dir ), len );
		
		dir = dx::XMVectorAdd( src, dir );

		dx::XMStoreFloat3( &vertbuf[1].Attribute<Vert::VertexLayout::Position_3D>(), dir );
	}
private:
	Vert::VertexBuffer vertbuf;
	Binds::DynamicVertexBuffer* dynamicVB = nullptr;
	float len;
};