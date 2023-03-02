#include "ConstantBufferEx.h"
#include "TechniqueProbe.h"

void Binds::PSConstantBufferEx::Bind( Graphics& gfx )
{
	pGetContext( gfx )->PSSetConstantBuffers( slot_, 1u, pConstBuffer.GetAddressOf() );
}

void Binds::PSConstantBufferEx::Update( Graphics& gfx, const CB::Buffer& buffer )
{
	D3D11_MAPPED_SUBRESOURCE msr;
	pGetContext( gfx )->Map( pConstBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD,
							 0u, &msr );
	memcpy( msr.pData, buffer.GetData(), buffer.sizeBytes() );
	pGetContext( gfx )->Unmap( pConstBuffer.Get(), 0u );
}

Binds::PSConstantBufferEx::PSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData )
	: slot_( slot )
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

/******************** Caching PS const buffer ************************************************/

Binds::CachingPSConstantBufferEx::CachingPSConstantBufferEx( Graphics& gfx, const CB::Buffer& buffer, UINT slot, bool setData )
	: PSConstantBufferEx( gfx, buffer, slot, setData )
	, buffer_( buffer )
	, synchronized_( true )
{}

const CB::Buffer& Binds::CachingPSConstantBufferEx::GetBuffer() const
{
	return buffer_;
}

void Binds::CachingPSConstantBufferEx::SetBuffer( const CB::Buffer& buffer )
{
	buffer_.CopyFrom( buffer );
	synchronized_ = false;
}

void Binds::CachingPSConstantBufferEx::Bind( Graphics& gfx )
{
	if ( !synchronized_ )
	{
		Update( gfx, buffer_ );
		synchronized_ = true;
	}
	PSConstantBufferEx::Bind( gfx );
}

void Binds::CachingPSConstantBufferEx::Accept( TechniqueProbe& probe )
{
	if ( probe.VisitBuffer( buffer_ ) )
	{ // Buffer changed
		synchronized_ = false;
	}
}
