#pragma once
#include <DirectXMath.h>

namespace MathUtil
{
	
};

static DirectX::XMFLOAT3 operator+( const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs )
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

static bool operator==( const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs )
{
	return { lhs.x == rhs.x &&  lhs.y  == rhs.y && lhs.z == rhs.z };
}