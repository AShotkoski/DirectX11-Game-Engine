#include "Mesh.h"
#include "Binds/TransformationConstBuffer.h"

namespace dx = DirectX;

Mesh::Mesh( Graphics& gfx, const Material& material, const aiMesh& mesh )
	: Drawable(gfx,material,mesh)
{
}

void Mesh::BindTransform( DirectX::XMMATRIX Transform )
{
	Transform_ = Transform;
}

DirectX::XMMATRIX Mesh::GetTransformationMatrix() const noexcept
{
	// Return concatenated matrix from all parents
	return Transform_;
}