#include "VertexBuffer.h"

namespace Binds
{

	VertexBuffer::VertexBuffer( Graphics& gfx, const Vert::VertexBuffer& vb )
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

};