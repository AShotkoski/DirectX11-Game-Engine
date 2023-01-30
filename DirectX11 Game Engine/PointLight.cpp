#include "PointLight.h"
#include "ImGui/imgui.h"

PointLight::PointLight( Graphics& gfx, float radius )
	:
	cbuf(gfx),
	LightPos(0,0,0),
	mesh(gfx, radius)
{
	
}

void PointLight::Bind( Graphics& gfx ) const
{
	PointLightCBuf ConstBuffer =
	{
		LightPos
	};

	cbuf.Update( gfx, ConstBuffer );

	cbuf.Bind( gfx );
}

// if draw is not called there is not a performace downside to holding a mesh for the light
void PointLight::Draw( Graphics& gfx ) const
{
	mesh.SetPos( LightPos );
	mesh.Draw( gfx );
}

void PointLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Light" ) )
	{
		ImGui::DragFloat( "x", &LightPos.x, 0.075f );
		ImGui::DragFloat( "y", &LightPos.y, 0.075f );
		ImGui::DragFloat( "z", &LightPos.z, 0.075f );
	}
	ImGui::End();
}
