#include "Step.h"
#include "FrameCommander.h"


Step::Step( size_t passNum )
	: passIdx(passNum)
{
}

void Step::Submit( FrameCommander& frame, const Drawable& drawable )
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
