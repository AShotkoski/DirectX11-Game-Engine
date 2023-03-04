#include "PointLight.h"
#include "ImGui.h"

PointLight::PointLight( Graphics& gfx, float radius, DirectX::XMFLOAT3 pos )
	:
	cbuf(gfx),
	LightPos(pos),
	mesh(gfx, radius)
{}

void PointLight::Bind( Graphics& gfx ) const
{
	PointLightCBuf LightProperties = {};
	LightProperties.ambient = ambient;;
	LightProperties.diffuseColor = diffuseColor;
	LightProperties.diffuseIntensity = diffuseIntensity;
	LightProperties.attenConst = attenConst;
	LightProperties.attenLin = attenLin;
	LightProperties.attenQuad = attenQuad;
	LightProperties.LightPos = LightPos;
		
	cbuf.Update( gfx, LightProperties );

	cbuf.Bind( gfx );
}

void PointLight::Draw( FrameCommander& frame ) const
{
	mesh.SetPos( LightPos );
	mesh.Submit( frame );
}

void PointLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Light" ) )
	{
		ImGui::DragFloat( "x", &LightPos.x, 0.075f );
		ImGui::DragFloat( "y", &LightPos.y, 0.075f );
		ImGui::DragFloat( "z", &LightPos.z, 0.075f );
		ImGui::ColorEdit3( "Diffuse", &diffuseColor.x );
		ImGui::ColorEdit3( "Ambient", &ambient.x );
		ImGui::DragFloat( "Intensity", &diffuseIntensity, 0.005f );
	}
	ImGui::End();
}
