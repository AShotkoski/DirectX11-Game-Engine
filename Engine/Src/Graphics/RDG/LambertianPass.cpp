#include "LambertianPass.h"
#include "Sink.h"
#include "Source.h"
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>

namespace RDG
{

	LambertianPass::LambertianPass( Graphics& gfx, const std::string& name )
		: RenderQueuePass( name )
	{
		RegisterSink( BufferSink<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		RegisterSink( BufferSink<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
	}

}