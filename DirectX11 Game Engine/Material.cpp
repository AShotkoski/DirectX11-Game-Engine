#include "Material.h"

Material::Material()
	: specularIntensity_(0.f)
	, specularPower_(1.f)
{
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
	return specular_power( level );
}

std::string Material::GetUID() const
{
	using namespace std::string_literals;
	std::string uid(
		std::to_string( specularIntensity_ ) + ":"s + std::to_string( specularPower_ ) );
}

void Material::parseAIMat( const aiMaterial& aiMat )
{
	float flBuf = 0.f;
	aiColor3D colBuf;
	if ( aiMat.Get( AI_MATKEY_SHININESS, flBuf ) == aiReturn_SUCCESS )
		specular_power( flBuf );
	if ( aiMat.Get( AI_MATKEY_COLOR_SPECULAR, colBuf ) == aiReturn_SUCCESS )
	{
		// Average color of specular color is our intensity, maybe add support for 
		// actual specular color in future but idk why.
		Color specCol = *reinterpret_cast<Color*>( &colBuf );
		flBuf = ( specCol.el[0] + specCol.el[1] + specCol.el[2] ) / 3.f;
		specular_intensity( flBuf );
	}
}
