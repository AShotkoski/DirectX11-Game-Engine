#pragma once

class Technique;
class Step;

namespace CB
{
	class Buffer;
}

class TechniqueProbe
{
public:
	void SetTechnique( const Technique* in_pTech )
	{
		pTechnique = in_pTech;
		OnSetTechnique();
	}
	void SetStep( const Step* in_pStep )
	{
		pStep = in_pStep;
		OnSetStep();
	}

	// Called by object containing a Dynamic CB
	// On implementation it is required that the VisitBuffer function returns true if the
	// contents of the buffer were modified at all.
	virtual bool VisitBuffer( CB::Buffer& ) const = 0;
protected:
	virtual void OnSetTechnique() {}
	virtual void OnSetStep() {}
protected:
	const Technique* pTechnique = nullptr;
	const Step* pStep = nullptr;
};