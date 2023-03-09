#pragma once

#include <Graphics/GraphicsResource.h>
#include <Graphics/Graphics.h>
#include <memory>

class Bindable : public GraphicsResource
{
public:
	virtual void Bind( Graphics& gfx ) = 0;
	virtual void InitParentRefs( const class Drawable& parent ) {}; 
	virtual void Accept( class TechniqueProbe& ) {};
	virtual ~Bindable() = default;

	// In order to register child bindable with the BindableCodex you must implement the
	// following, if the example ctor was ChildBind(Graphics&, bool*, std::string):
	// static std::string GenerateUID(bool*, std::string);
	// static std::shared_ptr<ChildBind> Resolve(Graphics&, bool*, std::string);
};

