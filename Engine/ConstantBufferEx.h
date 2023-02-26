#pragma once
#include "Bindable.h"
#include "DynamicCB.h"

#include "Macros.h"

namespace Binds
{
	// No common class for CB because that's a little much on the inheritance

	class PSConstantBufferEx : public Bindable
	{
	public:
		virtual void Bind( Graphics& gfx ) override
		{
			pGetContext( gfx )->PSSetConstantBuffers( slot_, 1u, pConstBuffer.GetAddressOf() );
		}
		void Update( Graphics& gfx, const CB::Buffer& buffer )
		{
			D3D11_MAPPED_SUBRESOURCE msr;
			pGetContext( gfx )->Map( pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD,
									 0u, &msr );
			memcpy( msr.pData, buffer.GetData(), buffer.sizeBytes());
			pGetContext( gfx )->Unmap( pConstBuffer.Get(), 0u );
		}
	protected:
		PSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData )
			: slot_(slot)
		{
			HRESULT hr;

			D3D11_BUFFER_DESC cbd = { 0 };
			cbd.ByteWidth = (UINT)buffer.sizeBytes();
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.StructureByteStride = 0u;
			THROW_FAILED_GFX( pGetDevice( gfx )->CreateBuffer( &cbd, nullptr, &pConstBuffer ) );
			if ( setData )
			{
				Update( gfx, buffer );
			}
		}
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
		UINT slot_;
	};

	class CachingPSConstantBufferEx : public PSConstantBufferEx
	{
	public:
		CachingPSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData = true )
			: PSConstantBufferEx(gfx,buffer,slot, setData)
			, buffer_(buffer)
			, synchronized_(true)
		{}
		const CB::Buffer& GetBuffer() const
		{
			return buffer_;
		}
		void SetBuffer( const CB::Buffer& buffer )
		{
			buffer_.CopyFrom( buffer );
			synchronized_ = false;
		}
		virtual void Bind( Graphics& gfx ) override
		{
			if ( !synchronized_ )
			{
				Update( gfx, buffer_ );
				synchronized_ = true;
			}
			PSConstantBufferEx::Bind( gfx );
		}
	private:
		CB::Buffer buffer_;
		bool synchronized_;
	};
}