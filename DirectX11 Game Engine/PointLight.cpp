#include "PointLight.h"
#include "ImGui/imgui.h"

PointLight::PointLight( Graphics& gfx )
	:
	cbuf(gfx),
	LightPos(0,0,0)
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

void PointLight::SpawnControlWindow()
{
	if ( ImGui::Begin( "Light" ) )
	{
		ImGui::DragFloat( "x", &LightPos.x );
		ImGui::DragFloat( "y", &LightPos.y );
		ImGui::DragFloat( "z", &LightPos.z );
	}
	ImGui::End();
}
