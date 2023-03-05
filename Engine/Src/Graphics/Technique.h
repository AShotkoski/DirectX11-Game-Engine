#pragma once
#include "Step.h"
#include <vector>
#include <string>

class Drawable;

class Technique
{
public:
	Technique( const std::string& in_name = "unnamed");
	void Submit( class FrameCommander& frame, const Drawable& drawable ) const;
	void AddStep( Step step );
	void InitParentRef( const Drawable& parent );
	void Accept( class TechniqueProbe& probe );
	bool Active() const;
	void SetActiveState( bool active );
	const std::string& GetName() const;
private:
	bool isActive = true;
	std::string name;
	std::vector<Step> steps;
};

