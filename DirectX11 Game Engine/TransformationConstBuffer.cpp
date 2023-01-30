#include "TransformationConstBuffer.h"

TransformationConstBuffer::TransformationConstBuffer( Graphics& gfx, const Drawable& parent )
	:
	VertexCBuf(gfx),
	parent(parent)
{
}

void TransformationConstBuffer::Bind( Graphics& gfx )
{
	// Update vertex const buffer with projected transformation matrix given by 
	// owner drawable
	const auto parentModel = parent.GetTransformationMatrix();

	const TransformBuffer tb          = {
        DirectX::XMMatrixTranspose( parentModel ), // model
        DirectX::XMMatrixTranspose(
            parentModel * gfx.GetCamera().GetMatrix() * gfx.GetProjection() ) }; // modelviewproj

	VertexCBuf.Update( gfx, tb);
	VertexCBuf.Bind( gfx );
}
