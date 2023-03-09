#pragma once
#include <Graphics/Graphics.h>

// Interface class that allows children access to the Direct3D device and context

class GraphicsResource
{
protected:
	static ID3D11Device* pGetDevice( Graphics& gfx ) noexcept;
	static ID3D11DeviceContext* pGetContext( Graphics& gfx ) noexcept;
};

