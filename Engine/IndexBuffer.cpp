#include "IndexBuffer.h"
#include "Macros.h"
#include "BindableCodex.h"
#include <cassert>

namespace Binds
{

	IndexBuffer::IndexBuffer( Graphics& gfx, const std::vector<unsigned short>& indices, std::string tag )
	{

		nIndices = (UINT)indices.size();
		assert( nIndices > 0 );
		// Errors
		HRESULT hr;

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = UINT( sizeof( unsigned short ) * nIndices );
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof( indices[0] );
		D3D11_SUBRESOURCE_DATA isrd = { 0 };
		isrd.pSysMem = indices.data();
		isrd.SysMemPitch = 0u; // Texture
		isrd.SysMemSlicePitch = 0u;

		THROW_FAILED_GFX( pGetDevice( gfx )->CreateBuffer( &ibd, &isrd, &pIndexBuffer ) );

	}

	void IndexBuffer::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );
	}

	UINT IndexBuffer::GetIndicesCount() const noexcept
	{
		assert(
			"Tried getting indices count before initializing index buffer."
			&& pIndexBuffer.GetAddressOf() != nullptr );
		return nIndices;
	}

	std::string IndexBuffer::GenerateUID( const std::vector<unsigned short>& indices, std::string tag )
	{
		using namespace std::string_literals;
		return std::string( typeid( IndexBuffer ).name() + "_"s + tag );
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(
		Graphics&                          gfx,
		const std::vector<unsigned short>& indices,
		std::string                        tag )
	{
		return Codex::Resolve<IndexBuffer>(gfx, indices, tag);
	}

};