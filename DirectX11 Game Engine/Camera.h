#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow();
	void Reset() noexcept;
	void UpdateView( DirectX::XMFLOAT2 dView );
private:
	DirectX::XMFLOAT3 Position;
	float pitch;
	float yaw;
};

