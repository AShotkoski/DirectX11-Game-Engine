#pragma once
#include "Step.h"
#include "Drawable.h"
#include "Step.h"

// pDraw, pStep
class Job
{
public:
	std::shared_ptr<Drawable> pDrawable;
	std::shared_ptr<Step> pStep;
};

