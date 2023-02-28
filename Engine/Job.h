#pragma once

class Step;
class Drawable;

// pDraw, pStep
class Job
{
public:
	Job( const Drawable* pDraw, const Step* pStep );
	void Execute(class Graphics& gfx) const;
private:
	const Drawable* pDrawable;
	const Step* pStep;
};

