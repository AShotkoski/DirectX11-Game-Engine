#pragma once
#include "Drawable.h"

class Cube : public DrawableBase<Cube>
{
public:
	Cube(
		Graphics&         gfx,
		float             size,
		float             rho,
		float             theta,
		float             phi,
		DirectX::XMFLOAT3 matColor,
		float             specInt,
		float             specPow );
	Cube(
		Graphics&         gfx,
		float             size,
		float             rho,
		float             theta,
		float             phi,
		float             dTheta,
		float             dPhi,
		float             dPitch,
		float             dYaw,
		float             dRoll,
		DirectX::XMFLOAT3 matColor,
		float             specInt,
		float             specPow );
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
	void              Update( float dt );
	void setSize( float x, float y, float z )
	{
		sizeX = x;
		sizeY = y;
		sizeZ = z;
	}
private:
	float size;
	float sizeX = -1;
	float sizeY = -1;
	float sizeZ = -1;

	// Position
	float rho;
	float theta;
	float phi;
	// Rotation
	float pitch = 0;
	float yaw   = 0;
	float roll  = 0;

	// Derivates //
	// Position
	float dTheta = 0;
	float dPhi   = 0;
	// Rotation
	float dPitch = 0;
	float dYaw   = 0;
	float dRoll  = 0;
};
