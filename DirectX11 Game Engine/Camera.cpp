#include "Camera.h"
#include "ImGui/imgui.h"

using namespace DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	// Get Camera Pos
	const auto pos = XMVector3Transform(
		XMVectorSet( 0, 0, -rho, 1.f ),
		XMMatrixRotationRollPitchYaw( phi, -theta, 0 ) );
	// Look where we want
	return (DirectX::XMMatrixLookAtLH( pos, XMVectorZero(), XMVectorSet( 0, 1, 0, 1 ) ) )
		* XMMatrixRotationRollPitchYaw( pitch, -yaw, roll );
}

void Camera::SpawnControlWindow()
{
	if( ImGui::Begin( "Camera" ) )
	{
		ImGui::SliderFloat( "Rho", &rho, 0.5f, 50.f );
		ImGui::SliderAngle( "Theta", &theta, -180.f, 180.f );
		ImGui::SliderAngle( "Phi", &phi, -89.f, 89.f );
		ImGui::NewLine();
		ImGui::SliderAngle( "Pitch", &pitch, -89.f, 89.f );
		ImGui::SliderAngle( "Yaw", &yaw, -89.f, 89.f );
		ImGui::SliderAngle( "Roll", &roll, -45.f, 45.f );
		if( ImGui::Button( "Reset" ) )
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	rho   = 10.f;
	phi   = 0.f;
	theta = 0.f;
	pitch = 0.0f;
	yaw   = 0.0f;
	roll  = 0.0f;
}
