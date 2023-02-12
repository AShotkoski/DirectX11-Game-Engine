#pragma once
#include "Drawable.h"
#include "Material.h"

class Cube : public Drawable
{
public:
	Cube(
		Graphics&         gfx,
		DirectX::XMFLOAT3 size,
		DirectX::XMFLOAT3 position,
		float pitch, float yaw, float roll,
		Material material );
	DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
	void              Update( float dt );
private:
	DirectX::XMFLOAT3 size;
	Material mat;
	// Position
	DirectX::XMFLOAT3 pos;
	// Rotation
	float pitch = 0;
	float yaw   = 0;
	float roll  = 0;
	
};
