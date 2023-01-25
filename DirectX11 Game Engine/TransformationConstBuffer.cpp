#include "TransformationConstBuffer.h"

TransformationConstBuffer::TransformationConstBuffer( Graphics& gfx, const Drawable& parent )
	:
	VertexCBuf(gfx,parent.GetTransformationMatrix()),
	parent(parent)
{
}

void TransformationConstBuffer::Bind( Graphics& gfx )
{
	// Update vertex const buffer with projected transformation matrix given by 
	// owner drawable
	VertexCBuf.Update( gfx, 
					   DirectX::XMMatrixTranspose
					   (
						   parent.GetTransformationMatrix() 
						   * gfx.GetProjection()
					   )
	);
	VertexCBuf.Bind( gfx );
}
