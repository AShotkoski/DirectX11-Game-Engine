#include "RenderQueuePass.h"
#include "Sink.h"
#include "Source.h"
#include <Graphics/Job.h>
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>

namespace RDG 
{
	RenderQueuePass::RenderQueuePass( const std::string& name )
		: BindingPass(name)
	{
		RegisterSink( BufferSink<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		RegisterSink( BufferSink<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
	}
	void RenderQueuePass::Execute( Graphics& gfx )
	{
		pTargetBuffer->Bind( gfx, *pTargetDepthBuffer );
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