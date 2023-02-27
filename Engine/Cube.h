#pragma once
#include "Drawable.h"
#include "Mesh.h"
#include "Material.h"
#include <filesystem>

class Cube : public Drawable
{
public:
	Cube(
		Graphics&         gfx,
		DirectX::XMFLOAT3 size,
		DirectX::XMFLOAT3 position,
		float pitch, float yaw, float roll);
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
private:
	DirectX::XMFLOAT3 size;
	// Position
	DirectX::XMFLOAT3 pos;
	// Rotation
	float pitch = 0;
	float yaw   = 0;
	float roll  = 0;
	
};
