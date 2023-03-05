#pragma once

#include "Graphics/Graphics.h"
#include <memory>

class Bindable
{
public:
	virtual void Bind( Graphics& gfx ) = 0;
	// Must implement static std::string GenerateUID(), that takes the same params as the constructor minus the gfx
	// Must implement static std::shared_ptr<Bindable> Resolve(), that takes same params as ctor
	// Must implement std::string GetUID(), that takes same params as ctor minus gfx
	virtual void InitParentRefs( const class Drawable& parent ) {}; 
	virtual void Accept( class TechniqueProbe& ) {};
	virtual ~Bindable() = default;
protected:
	// Bindable is friend class to graphics, and these static member functions will let
	// the children of Bindable access specific private member data of graphics
	static ID3D11Device* pGetDevice(Graphics& gfx) noexcept;
	static ID3D11DeviceContext* pGetContext(Graphics& gfx) noexcept;
};

