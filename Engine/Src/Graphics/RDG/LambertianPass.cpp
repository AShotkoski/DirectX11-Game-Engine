#include "LambertianPass.h"
#include "Sink.h"
#include "Source.h"
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>
#include <Binds/Stencil.h>
#include <Binds/Rasterizer.h>

namespace RDG
{

	LambertianPass::LambertianPass( Graphics& gfx, const std::string& name )
		: RenderQueuePass( name )
	{
		RegisterSink( BufferSink<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		RegisterSink( BufferSink<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		AddBind( Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Off ));
	}

}