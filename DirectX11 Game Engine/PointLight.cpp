#include "PointLight.h"

PointLight::PointLight( Graphics& gfx )
	:
	cbuf(gfx)
{
	
}

void PointLight::Bind( Graphics& gfx ) const
{
	PointLightCBuf ConstBuffer =
	{
		{0,0,0 } // Pos
	};

	cbuf.Update( gfx, ConstBuffer );

	cbuf.Bind( gfx );
}
