#include "Camera.h"
#include "ImGui.h"
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
		ImGui::Checkbox( "Show Info", &im_showDebugInfo );
		if ( im_showDebugInfo )
		{
			ImGui::TextColored( ImColor( 255, 127, 0 ), "Position:" );
			ImGui::Text( "(%.2f, %.2f, %.2f)", Position.x, Position.y, Position.z );
			ImGui::TextColored( ImColor( 255, 127, 0 ), "Orientation:" );
			ImGui::Text( "Pitch: %.2f, Yaw: %.2f", pitch * 180.f / XM_PI, yaw * 180.f / XM_PI );
		}
		if( ImGui::Button( "Reset" ) )
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	Position.x = -6.f;
	Position.y = 4.f;
	Position.z = -3.f;
	pitch      = 0.087f;
	yaw        = 1.31f;
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

void Camera::UpdatePosition( DirectX::XMFLOAT3 dPos, float dt )
{
	// Lets not fly around at 7000 fps anymore
	const float MoveSpeed = this->MoveSpeed * dt;

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

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return Position;
}

DirectX::XMFLOAT3 Camera::GetDirectionVector() const
{
	XMFLOAT3X3 v;
	XMStoreFloat3x3( &v, view );

	return DirectX::XMFLOAT3(v(0,2), v(1,2), v(2,2));
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
