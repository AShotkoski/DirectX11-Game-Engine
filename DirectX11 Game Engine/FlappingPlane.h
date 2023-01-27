#pragma once
#include "Drawable.h"

class FlappingPlane : public DrawableBase<FlappingPlane>
{
public:
	FlappingPlane( Graphics& gfx );

	void Update( float dt ) override;
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	float time = 0;
};

