#include "Technique.h"
#include "FrameCommander.h"
#include "TechniqueProbe.h"

Technique::Technique( const std::string& in_name )
	:name(in_name)
{
}

void Technique::Submit( FrameCommander& frame, const Drawable& drawable ) const
{
	// Only submit to steps if tech is active
	if ( !isActive )
	{
		return;
	}

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

void Technique::Accept( TechniqueProbe& probe )
{
	probe.SetTechnique( this );
	for ( auto& s : steps )
	{
		s.Accept( probe );
	}
}

bool Technique::Active() const
{
	return isActive;
}

void Technique::SetActiveState( bool active )
{
	isActive = active;
}

const std::string& Technique::GetName() const
{
	return name;
}
