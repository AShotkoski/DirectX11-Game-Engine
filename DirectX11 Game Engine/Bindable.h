#pragma once

#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind( Graphics& gfx ) = 0;
	virtual ~Bindable() = default;
protected:
	// Bindable is friend class to graphics, and these static member functions will let
	// the children of Bindable access specific private member data of graphics
	static ID3D11Device* pGetDevice(Graphics& gfx) noexcept;
	static ID3D11DeviceContext* pGetContext(Graphics& gfx) noexcept;
};

