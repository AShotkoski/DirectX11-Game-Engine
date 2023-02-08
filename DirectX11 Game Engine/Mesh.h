#pragma once
#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include <assimp/BaseImporter.h>

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( Graphics& gfx, const std::wstring& filename )
	{
		if ( !isStaticInitialized() )
		{
			AddStaticBind( std::make_unique<Binds::Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

			// load verts and 
			
			
			AddStaticBind( std::make_unique<Binds::PixelShader>( gfx, L"PSPhong.cso" ) );

			
			auto vs = std::make_unique<Binds::VertexShader>( gfx, L"VSPhong.cso" );
			auto vsbtyecode = vs->pGetBytecode();
			AddStaticBind( std::move( vs ) );

		}

			AddBind( std::make_unique<Binds::TransformationConstBuffer>( gfx, *this ) ); struct PSCBuf
			{
				DirectX::XMFLOAT3 MatCol;
				float specularIntensity;
				alignas( 16 ) float specularPower;
			};

			PSCBuf cubeProps = { {1,1,1}, 1, 50 };

			AddBind( std::make_unique<Binds::PixelConstantBuffer<PSCBuf>>( gfx, cubeProps, 1u ) );
	}
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override
	{
		return DirectX::XMMatrixIdentity();
	};
	virtual void Update( float dt ) override
	{};
};