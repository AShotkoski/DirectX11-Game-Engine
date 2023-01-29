#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow();
	void Reset() noexcept;
private:
	float rho;
	float theta;
	float phi;
	float pitch;
	float yaw;
	float roll;
};

