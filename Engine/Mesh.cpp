#include "Mesh.h"
#include "TransformationConstBuffer.h"

namespace dx = DirectX;

Mesh::Mesh( std::vector<std::shared_ptr<Bindable>>&& binds, Graphics& gfx )
{
	/*
	AddBind( Binds::TransformationConstBuffer::Resolve( gfx, *this ) );
	for ( auto& b : binds )
	{
		AddBind( std::move( b ) );
	}
	*/
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