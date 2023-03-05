#include "Job.h"
#include "Drawable/Drawable.h"
#include "Step.h"

Job::Job( const Drawable* pDraw, const Step* pStep )
	: pStep{ pStep }
	, pDrawable{ pDraw }
{
}

void Job::Execute(Graphics& gfx) const
{
	pDrawable->Bind( gfx );
	pStep->Bind( gfx );
	gfx.DrawIndexed( pDrawable->GetIndexCount() );
}
