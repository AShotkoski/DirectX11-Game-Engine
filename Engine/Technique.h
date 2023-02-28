#pragma once
#include "Step.h"
#include <vector>

class Technique
{
public:
	void Submit( class FrameCommander& frame, const class Drawable& drawable );
	void AddStep( Step step );
private:
	std::vector<Step> steps;
	
};

