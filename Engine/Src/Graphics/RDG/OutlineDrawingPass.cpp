#include "OutlineDrawingPass.h"
#include <Binds/Stencil.h>
#include "Source.h"
#include <Binds/NullPixelShader.h>
#include "Sink.h"
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>

namespace RDG
{
	OutlineDrawingPass::OutlineDrawingPass( Graphics& gfx )
		: RenderQueuePass( "outlinedraw" )
	{
		RegisterSink( BufferSink<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		RegisterSink( BufferSink<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<RenderTarget>::MakeUnique( "rendertarget", pTargetBuffer ) );
		AddBind( Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Mask ) );
	}

}