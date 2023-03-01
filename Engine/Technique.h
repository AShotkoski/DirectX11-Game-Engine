#pragma once
#include "Step.h"
#include <vector>

class Drawable;

class Technique
{
public:
	void Submit( class FrameCommander& frame, const Drawable& drawable ) const;
	void AddStep( Step step );
	void InitParentRef( const Drawable& parent );
private:
	std::vector<Step> steps;
};

