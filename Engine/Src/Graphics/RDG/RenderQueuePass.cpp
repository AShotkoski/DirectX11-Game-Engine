#include "RenderQueuePass.h"
#include "Sink.h"
#include "Source.h"
#include <Graphics/Job.h>

namespace RDG 
{
	RenderQueuePass::RenderQueuePass( const std::string& name )
		: BindingPass(name)
	{
	}
	void RenderQueuePass::Execute( Graphics& gfx )
	{
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