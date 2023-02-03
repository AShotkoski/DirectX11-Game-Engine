#include "Camera.h"
#include "ImGui/imgui.h"

using namespace DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	// Look where we want
	return XMMatrixTranslationFromVector( XMLoadFloat3( &Position ) ) 
		* XMMatrixRotationRollPitchYaw( pitch, -yaw, 0.f );

}

void Camera::SpawnControlWindow()
{
	if( ImGui::Begin( "Camera" ) )
	{
		ImGui::DragFloat( "X", &Position.x, 0.5f,  0.5f, 50.f );
		ImGui::DragFloat( "Y", &Position.y, 0.5f, 0.5f, 50.f );
		ImGui::DragFloat( "Z", &Position.z, -0.5f, 0.5f, 50.f );
		ImGui::NewLine();
		if( ImGui::Button( "Reset" ) )
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	Position.x = 0.f;
	Position.y = 0.f;
	Position.z = 10.f;
	pitch      = 0.f;
	yaw        = 0.f;
}

void Camera::UpdateView( DirectX::XMFLOAT2 dView )
{
	pitch += dView.y;
	yaw += dView.x;
}
