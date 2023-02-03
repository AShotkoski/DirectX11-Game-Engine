#include "Camera.h"
#include "ImGui/imgui.h"

using namespace DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	// Rotate where we want to look
	XMVECTOR base = XMVectorSet( 0, 0, 1.f, 0.f ); // Default is looking forward in z
	XMVECTOR lookat = XMVector3Transform(base, XMMatrixRotationRollPitchYaw( -pitch, yaw, 0.f ));

	auto xmpos = XMLoadFloat3( &Position );

	return XMMatrixLookAtLH(xmpos, lookat + xmpos, XMVectorSet(0,1.f,0,0.f));
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
	Position.z = -10.f;
	pitch      = 0.f;
	yaw        = 0.f;
}

void Camera::UpdateView( DirectX::XMFLOAT2 dView )
{
	pitch += dView.y * Sensitivity;
	yaw += dView.x * Sensitivity;
}
