#include "RenderQueuePass.h"
#include <Graphics/Job.h>
#include "Sink.h"
#include "Source.h"

namespace RDG 
{
	RenderQueuePass::RenderQueuePass( const std::string& name )
		: BindingPass(name)
	{
	}
	void RenderQueuePass::Execute( Graphics& gfx )
	{
		BindAll( gfx );
		for ( auto& j : jobs )
		{
			j.Execute( gfx );
		}
	}

	void RenderQueuePass::Accept( Job job )
	{
		jobs.push_back( std::move( job ) );
	}

	void RenderQueuePass::Clear()
	{
		jobs.clear();
	}
}