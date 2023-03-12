#include "RenderGraph.h"
#include "Sink.h"
#include "Source.h"
#include <Graphics/BufferResource.h>
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>

RDG::RenderGraph::RenderGraph(Graphics& gfx, const std::string graph_name )
	: name( graph_name )
	, targetBuffer(gfx.pGetRenderTarget())
	, masterDS(std::make_shared<Binds::DepthStencil>(gfx, gfx.GetWidth(), gfx.GetHeight() ))
{
	// Register globals
	RegisterGlobalSink( BufferSink<RenderTarget>::MakeUnique( "backbuffer", targetBuffer ));
	RegisterGlobalSource( BufferSource<RenderTarget>::MakeUnique( "backbuffer", targetBuffer ) );
	RegisterGlobalSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", masterDS ) );
}
