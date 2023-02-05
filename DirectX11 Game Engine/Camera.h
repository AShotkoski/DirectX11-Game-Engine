#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	DirectX::XMMATRIX GetInvMatrix() const noexcept;
	void SpawnControlWindow();
	void Reset() noexcept;
	void UpdateView( DirectX::XMFLOAT2 dView );
	void MovePosition( DirectX::XMFLOAT3 dPos );
	void EnableMouseControl();
	void DisableMouseControl();
	bool isMouseControlEnabled() const;
	void UpdateMovementSpeed( float factor );
private:
	void CalculateMatrices();
private:
	DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX invView = DirectX::XMMatrixIdentity();
	bool isMouseControl = false;
	DirectX::XMFLOAT3 Position;
	float pitch;
	float yaw;
	// Only adjust the numerator
	static constexpr float Sensitivity = 25.f / 10000.f;
	float MoveSpeed = 35.f / 1000.f;
};

