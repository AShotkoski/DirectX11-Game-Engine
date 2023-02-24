#pragma once
#include "Colors.h"
#include "ConstantBuffers.h"
#include <assimp/material.h>
#include <DirectXMath.h>
#include <memory>

class Material
{
public:
	Material();
	Material& specular_intensity( float intensity );
	Material& specular_power( float power );
	Material& shininess( float level );
	std::string GetUID() const;
	void parseAIMat( const aiMaterial& aiMat );
private:
	// Constant buffer data
	float specularIntensity_;
	float specularPower_;
	float padding[2];
};
