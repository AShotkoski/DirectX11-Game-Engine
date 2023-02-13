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
	return specular_power( level );
}

std::string Material::GetUID() const
{
	using namespace std::string_literals;
	Color tagC = color_;
	tagC.el[0] *= specularIntensity_ + 1.f - specularPower_;
	tagC.el[1] *= specularIntensity_ + 1.f - specularPower_;
	tagC.el[2] *= specularIntensity_ + 1.f - specularPower_;
	std::string uid(
		std::to_string( tagC.el[0] ) + ":"s + std::to_string( tagC.el[1] ) + ":"s
		+ std::to_string( tagC.el[2] ) );
	return uid;
}

void Material::parseAIMat( const aiMaterial& aiMat )
{
	float flBuf = 0.f;
	aiColor3D colBuf;
	if ( aiMat.Get( AI_MATKEY_SHININESS, flBuf ) == aiReturn_SUCCESS )
		specular_power( flBuf );
	if ( aiMat.Get( AI_MATKEY_COLOR_DIFFUSE, colBuf ) == aiReturn_SUCCESS )
		color( *reinterpret_cast<Color*>(&colBuf) );
	if ( aiMat.Get( AI_MATKEY_COLOR_SPECULAR, colBuf ) == aiReturn_SUCCESS )
	{
		// Average color of specular color is our intensity, maybe add support for 
		// actual specular color in future but idk why.
		Color specCol = *reinterpret_cast<Color*>( &colBuf );
		flBuf = ( specCol.el[0] + specCol.el[1] + specCol.el[2] ) / 3.f;
		specular_intensity( flBuf );
	}
}
