#include "Pass.h"
#include "Graphics.h"
#include "Job.h"


void Pass::Execute( Graphics& gfx ) const
{
	for ( auto& j : jobs )
	{
		j.Execute( gfx );
	}
}

void Pass::Accept( Job job )
{
	jobs.push_back( std::move( job ) );
}

void Pass::Clear()
{
	jobs.clear();
}
