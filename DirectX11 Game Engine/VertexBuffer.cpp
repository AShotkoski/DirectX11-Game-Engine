#include "VertexBuffer.h"

void VertexBuffer::Bind( Graphics& gfx )
{
	const UINT offset = 0u;
	pGetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
