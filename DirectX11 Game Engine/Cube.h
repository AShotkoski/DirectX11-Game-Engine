#pragma once
#include "Drawable.h"

class Cube : public Drawable
{
public:
	Cube( Graphics& gfx,float size, float rho, float theta, float phi );
	void Update( float dt ) override;
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	float size;
	// Position
	float rho;
	float theta;
	float phi;
	// Rotation
	float pitch = 0;
	float yaw = 0;
	float roll = 0;
	// Derivates
	float dPitch = 0;
	float dYaw = 0;
	float dRoll = 0;
};

