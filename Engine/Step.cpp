#include "Step.h"
#include "FrameCommander.h"
#include "TechniqueProbe.h"

Step::Step( size_t passNum )
	: passIdx(passNum)
{
}

void Step::Submit( FrameCommander& frame, const Drawable& drawable ) const
{
	frame.Accept( Job( &drawable, this ), passIdx );
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

void Step::Accept( TechniqueProbe& probe ) const
{
	probe.SetStep( this );
	for ( auto& b : pBinds )
	{
		b->Accept( probe );
	}
}
