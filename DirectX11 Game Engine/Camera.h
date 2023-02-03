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
	void EnableMouseControl();
	void DisableMouseControl();
	bool isMouseControlEnabled() const;
private:
	bool isMouseControl = false;
	DirectX::XMFLOAT3 Position;
	float pitch;
	float yaw;
	// Only adjust the numerator
	static constexpr float Sensitivity = 25.f / 10000.f;
};

