#pragma once
#include "Drawable.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>

// In this class it would be a great idea to upgrade the drawing so that we don't need to 
// re-calculate the concatenated matrices every single call to draw.

class Mesh : public Drawable
{
public:
	Mesh( Graphics& gfx, const Material& material, const aiMesh& mesh );
	void BindTransform( DirectX::XMMATRIX Transform );
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	DirectX::XMMATRIX Transform_ = DirectX::XMMatrixIdentity();
};