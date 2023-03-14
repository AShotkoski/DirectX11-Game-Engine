#include "Step.h"
#include "FrameCommander.h"
#include "TechniqueProbe.h"
#include "RDG/RenderQueuePass.h"
#include <log.h>
#include "Job.h"
#include "RDG/RenderGraph.h"

Step::Step( const std::string& target_pass )
	: targetPass(target_pass)
{
}

void Step::Submit( const Drawable& drawable ) const
{
	DCHECK_F( pTargetPass != nullptr, "Cannot submit to target pass that is not linked.");
	pTargetPass->Accept( Job{ &drawable, this } );
}

void Step::Bind( Graphics& gfx ) const
{
	for ( auto& b : pBinds )
	{
		b->Bind( gfx );
	}
}

void Step::AddBind( std::shared_ptr<Bindable> pBind )
{
	pBinds.push_back( std::move( pBind ) );
}

void Step::InitParentRef( const Drawable& parent )
{
	for ( auto& b : pBinds )
	{
		b->InitParentRefs( parent );
	}
}

void Step::Accept( TechniqueProbe& probe )
{
	probe.SetStep( this );
	for ( auto& b : pBinds )
	{
		b->Accept( probe );
	}
}

void Step::LinkTarget( RDG::RenderGraph& graph )
{
	pTargetPass = &graph.GetRenderQueue( targetPass );
}
