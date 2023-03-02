#pragma once
#include "Bindable.h"
#include <vector>

class Step
{
public:
	Step( size_t passNum );
	void Submit( class FrameCommander& frame, const class Drawable& drawable ) const;
	void Bind( Graphics& gfx ) const;
	void AddBind( std::shared_ptr<Bindable> pBind );
	void InitParentRef( const class Drawable& parent );
	void Accept( class TechniqueProbe& probe );
private:
	std::vector<std::shared_ptr<Bindable>> pBinds;
	size_t passIdx;
};

