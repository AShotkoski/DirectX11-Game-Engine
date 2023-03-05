#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Binds
{

	VertexBuffer::VertexBuffer( Graphics& gfx, const Vert::VertexBuffer& vb,std::string tag )
	{
		assert( vb.Size() > 0 );
		// Error checker
		HRESULT hr;

		D3D11_SUBRESOURCE_DATA srd = { 0 };
		srd.pSysMem = vb.GetData();
		srd.SysMemPitch = 0u; // Texture
		srd.SysMemSlicePitch = 0u;

		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = (UINT)( vb.SizeBytes() );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = (UINT)vb.VertexSizeBytes();

		THROW_FAILED_GFX( pGetDevice( gfx )->CreateBuffer( &bd, &srd, &pVertexBuffer ) );
		// Store stride based off templated vertex for use in binding
		stride = (UINT)vb.VertexSizeBytes();
	}

	void VertexBuffer::Bind( Graphics& gfx )
	{
		const UINT offset = 0u;
		pGetContext( gfx )->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
	}

	std::string VertexBuffer::GenerateUID( const Vert::VertexBuffer& vb, std::string tag )
	{
		using namespace std::string_literals;
		return std::string( typeid( VertexBuffer ).name() + "_"s + tag );
	}

	std::shared_ptr<VertexBuffer>
		VertexBuffer::Resolve( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag )
	{
		return Codex::Resolve<VertexBuffer>(gfx,vb,tag);
	}

	DynamicVertexBuffer::DynamicVertexBuffer(
		Graphics&                 gfx,
		const Vert::VertexBuffer& vb,
		std::string               tag )
	{
		assert( vb.Size() > 0 );
		// Error checker
		HRESULT hr;

		D3D11_SUBRESOURCE_DATA srd = { 0 };
		srd.pSysMem = vb.GetData();
		srd.SysMemPitch = 0u; // Texture
		srd.SysMemSlicePitch = 0u;

		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = (UINT)( vb.SizeBytes() );
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = (UINT)vb.VertexSizeBytes();

		THROW_FAILED_GFX( pGetDevice( gfx )->CreateBuffer( &bd, &srd, &pVertexBuffer ) );
		// Store stride based off templated vertex for use in binding
		stride = (UINT)vb.VertexSizeBytes();
	}

	void DynamicVertexBuffer::Update(Graphics& gfx, const Vert::VertexBuffer& vb ) const
	{
		// Get data on const buffer and lock gpu from accessing it to write data
		D3D11_MAPPED_SUBRESOURCE msr;
		pGetContext( gfx )->Map( pVertexBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD,
								 0u, &msr );
		memcpy( msr.pData, vb.GetData(), vb.SizeBytes() );
		pGetContext( gfx )->Unmap( pVertexBuffer.Get(), 0u );
	}

};