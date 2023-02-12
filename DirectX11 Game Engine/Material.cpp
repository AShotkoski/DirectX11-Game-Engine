#include "Material.h"

Material::Material()
	: color_(Colors::FloralWhite)
	, specularIntensity_(0.f)
	, specularPower_(1.f)
{
}

Material& Material::color( Color c )
{
	color_ = c;
	return *this;
}

Material& Material::color( float r, float g, float b )
{
	color_ = Color::MakeRgb( r, g, b );
	return *this;
}

Material& Material::color( DirectX::XMFLOAT3 c )
{
	color_ = *reinterpret_cast<Color*>( &c );
	return *this;
}

Material& Material::specular_intensity( float intensity )
{
	specularIntensity_ = intensity;
	return *this;
}

Material& Material::specular_power( float power )
{
	specularPower_ = power;
	return *this;
}

Material& Material::shininess( float level )
{
	return specular_power( ( 1.f / level ) * 10.f );
}
