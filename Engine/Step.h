#pragma once
#include "Bindable.h"
#include <vector>

class Step
{
public:
	Step( size_t passNum );
	void Submit( class FrameCommander& frame, const class Drawable& drawable );
	void AddBind( std::shared_ptr<Bindable> pBind );
private:
	std::vector<std::shared_ptr<Bindable>> pBinds;
	size_t passIdx;
};

