#pragma once
#include "Bindable.h"
#include "Macros.h"

namespace Binds
{

	template <typename CB>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer( Graphics& gfx, const CB& consts, UINT slot = 0u )
			:
			ConstantBuffer( gfx, slot )
		{
			Update( gfx, consts );
		}
		ConstantBuffer( Graphics& gfx, UINT slot = 0u )
			:
			slot( slot )
		{
			// Errors
			HRESULT hr;

			D3D11_BUFFER_DESC cbd = { 0 };
			cbd.ByteWidth = sizeof( CB );
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.StructureByteStride = 0u;
			THROW_FAILED_GFX( pGetDevice( gfx )->CreateBuffer( &cbd, nullptr, &pConstBuffer ) );
		}

		void Update( Graphics& gfx, const CB& consts )
		{
			// Get data on const buffer and lock gpu from accessing it to write data
			D3D11_MAPPED_SUBRESOURCE msr;
			pGetContext( gfx )->Map( pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD,
									 0u, &msr );
			memcpy( msr.pData, &consts, sizeof( consts ) );
			pGetContext( gfx )->Unmap( pConstBuffer.Get(), 0u );
		}
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
		UINT slot;
	};

	// Create separate classes for vertex and index const buffers since they bind to different parts
	// of the d3d pipeline
	template <typename CB>
	class VertexConstantBuffer : public ConstantBuffer<CB>
	{
		using Bindable::pGetContext;
		using ConstantBuffer<CB>::pConstBuffer;
		using ConstantBuffer<CB>::slot;
	public:
		using ConstantBuffer<CB>::ConstantBuffer;
		void Bind( Graphics& gfx ) override
		{
			pGetContext( gfx )->VSSetConstantBuffers( slot, 1u, pConstBuffer.GetAddressOf() );
		}
	};


	template <typename CB>
	class PixelConstantBuffer : public ConstantBuffer<CB>
	{
		using Bindable::pGetContext;
		using ConstantBuffer<CB>::pConstBuffer;
		using ConstantBuffer<CB>::slot;
	public:
		using ConstantBuffer<CB>::ConstantBuffer;
		void Bind( Graphics& gfx ) override
		{
			pGetContext( gfx )->PSSetConstantBuffers( slot, 1u, pConstBuffer.GetAddressOf() );
		}
	};

};