#include "Technique.h"

void Technique::Submit( FrameCommander& frame, const Drawable& drawable ) const
{
	frame.Accept(//job)
}

void Technique::AddStep( Step step )
{
	steps.push_back( std::move( step ) );
}
