#include "OutlineMaskingPass.h"
#include <Binds/Stencil.h>
#include "Source.h"
#include <Binds/NullPixelShader.h>
#include "Sink.h"
#include <Binds/DepthStencil.h>

namespace RDG
{
	OutlineMaskingPass::OutlineMaskingPass( Graphics& gfx )
		: RenderQueuePass( "outlinemask" )
	{
		RegisterSink( BufferSink<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		RegisterSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", pTargetDepthBuffer ) );
		AddBind( Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Write ) );
		AddBind( Binds::NullPixelShader::Resolve( gfx ) );
	}

}