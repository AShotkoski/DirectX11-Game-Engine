#include "Technique.h"
#include "FrameCommander.h"

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
