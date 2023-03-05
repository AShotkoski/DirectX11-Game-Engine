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
	void SetTechnique( Technique* in_pTech )
	{
		techIdx++;
		pTechnique = in_pTech;
		OnSetTechnique();
	}
	void SetStep( Step* in_pStep )
	{
		stepIdx++;
		pStep = in_pStep;
		OnSetStep();
	}

	// Called by object containing a Dynamic CB
	// On implementation it is required that the VisitBuffer function returns true if the
	// contents of the buffer were modified at all.
	bool VisitBuffer( CB::Buffer& buffer)
	{
		bufferIdx++;
		return OnVisitBuffer( buffer );
	}
protected:
	virtual void OnSetTechnique() {}
	virtual void OnSetStep() {}
	virtual bool OnVisitBuffer( CB::Buffer& ) const = 0;
protected:
	Technique* pTechnique = nullptr;
	Step* pStep = nullptr;
	size_t techIdx = 0;
	size_t stepIdx = 0;
	size_t bufferIdx = 0;
};