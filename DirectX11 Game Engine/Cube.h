#pragma once
#include "Drawable.h"

class Cube : public Drawable
{
public:
	Cube( Graphics& gfx );
	void Update( float dt ) override;
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	float theta = 0;
};

