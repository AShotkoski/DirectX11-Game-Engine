#include "Technique.h"
#include "FrameCommander.h"
#include "TechniqueProbe.h"

void Technique::Submit( FrameCommander& frame, const Drawable& drawable ) const
{
	for ( auto& s : steps )
	{
		s.Submit( frame, drawable );
	}
}

void Technique::AddStep( Step step )
{
	steps.push_back( std::move( step ) );
}

void Technique::InitParentRef( const Drawable& parent )
{
	for ( auto& s : steps )
	{
		s.InitParentRef(parent);
	}
}

void Technique::Accept( TechniqueProbe& probe ) const
{
	probe.SetTechnique( this );
	for ( auto& s : steps )
	{
		s.Accept( probe );
	}
}
