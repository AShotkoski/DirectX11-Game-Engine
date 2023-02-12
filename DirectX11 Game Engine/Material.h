#pragma once
#include "Colors.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>
#include <memory>

class Material
{
public:
	Material();
	Material& color( Color c );
	Material& color( float r, float g, float b );
	Material& color( DirectX::XMFLOAT3 c );
	Material& specular_intensity( float intensity );
	Material& specular_power( float power );
	Material& shininess( float level );
private:
	// Constant buffer data
	Color color_; // 3 floats
	float specularIntensity_;
	alignas(16)float specularPower_;

};