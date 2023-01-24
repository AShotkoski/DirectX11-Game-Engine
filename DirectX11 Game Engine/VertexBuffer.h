#pragma once
#include "Bindable.h"
#include "Macros.h"
#include <vector>
#include <wrl.h>

class VertexBuffer : public Bindable
{
public:
	template <class Vertex>
	VertexBuffer( Graphics& gfx, const std::vector<Vertex>& vertices );
	void Bind( Graphics& gfx ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
};

template<class Vertex>
inline VertexBuffer::VertexBuffer( Graphics& gfx, const std::vector<Vertex>& vertices )
{
	// Error checker
	HRESULT hr;

	D3D11_SUBRESOURCE_DATA srd = { 0 };
	srd.pSysMem                = vertices.data();
	srd.SysMemPitch            = 0u; // Texture
	srd.SysMemSlicePitch       = 0u;

	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth           = sizeof( Vertex ) * vertices.size();
	bd.Usage               = D3D11_USAGE_DEFAULT;
	bd.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags      = 0u;
	bd.MiscFlags           = 0u;
	bd.StructureByteStride = sizeof( Vertex );

	THROW_FAILED_GFX( pGetDevice(gfx)->CreateBuffer(&bd, &srd, &pVertexBuffer));
	// Store stride based off templated vertex for use in binding
	stride = sizeof( Vertex );
}
