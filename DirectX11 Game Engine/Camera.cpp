#include "Camera.h"
#include "ImGui/imgui.h"
#include <algorithm>

using namespace DirectX;

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	return view;
}

DirectX::XMMATRIX Camera::GetInvMatrix() const noexcept
{
	return invView;
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
	CalculateMatrices();
}

void Camera::UpdateView( DirectX::XMFLOAT2 dView )
{
	pitch += dView.y * Sensitivity;
	yaw += dView.x * Sensitivity;
	// clamp angles
	pitch = std::clamp( pitch, -XM_PIDIV2 * 0.995f, XM_PIDIV2 * 0.995f );
	//todo clamp yaw
	CalculateMatrices();
}

void Camera::MovePosition( DirectX::XMFLOAT3 dPos )
{
	// Update dPos to care about direction the camera is facing and the movespeed
	XMStoreFloat3( &dPos, XMVector3Transform(
		XMLoadFloat3( &dPos ),
		XMMatrixRotationRollPitchYaw( pitch, yaw, 0.0f ) *
		XMMatrixScaling( MoveSpeed, MoveSpeed, MoveSpeed )
	) );
	Position.x += dPos.x;
	Position.y += dPos.y;
	Position.z += dPos.z;
	CalculateMatrices();
}

void Camera::EnableMouseControl()
{
	isMouseControl = true;
}

void Camera::DisableMouseControl()
{
	isMouseControl = false;
}

bool Camera::isMouseControlEnabled() const
{
	return isMouseControl;
}

void Camera::UpdateMovementSpeed( float factor )
{
	assert( factor >= 0.f && factor < 100.f );
	MoveSpeed *= factor;
}

void Camera::CalculateMatrices()
{
	// Rotate where we want to look
	XMVECTOR base = XMVectorSet( 0, 0, 1.f, 0.f ); // Default is looking forward in z
	XMVECTOR lookat = XMVector3Transform( base, XMMatrixRotationRollPitchYaw( pitch, yaw, 0.f ) );

	auto xmpos = XMLoadFloat3( &Position );

	view = XMMatrixLookAtLH( xmpos, lookat + xmpos, XMVectorSet( 0, 1.f, 0, 0.f ) );
	auto viewDet = XMMatrixDeterminant( view );
	invView = XMMatrixInverse( &viewDet, view );
}
