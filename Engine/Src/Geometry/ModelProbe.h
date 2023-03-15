#pragma once
#include "Graphics/TechniqueProbe.h"

class ModelProbe : public TechniqueProbe
{
public:
	virtual void OnSetTechnique() {}
	virtual bool OnVisitBuffer( CB::Buffer& ) const = 0;
};