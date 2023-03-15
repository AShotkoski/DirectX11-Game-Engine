#pragma once
#include "RenderGraph.h"
#include "BufferClearingPass.h"
#include "RenderQueuePass.h"
#include <Binds/RenderTarget.h>
#include "LambertianPass.h"

namespace RDG
{
	class TestGraph : public RenderGraph
	{
	public:
		TestGraph( Graphics& gfx )
			: RenderGraph( gfx, "TestGraph" )
		{
			targetBuffer->Bind( gfx, *masterDS );
			{
				auto pass = std::make_unique<BufferClearingPass>( "clearRT" );
				pass->SetSinkLinkage( "buffer", "$.backbuffer" );
				AppendPass( std::move( pass ) );
			}
			{
				auto pass = std::make_unique<BufferClearingPass>( "clearDS" );
				pass->SetSinkLinkage( "buffer", "$.depthstencil" );
				AppendPass( std::move( pass ) );
			}
			{
				auto pass = std::make_unique<LambertianPass>(gfx, "lambertian" );
				pass->SetSinkLinkage( "rendertarget", "clearRT.buffer" );
				pass->SetSinkLinkage( "depthstencil", "clearDS.buffer" );
				AppendPass( std::move( pass ) );
			}
			LinkGlobalSink( "backbuffer", "clearRT.buffer" );
			Finalize();
		}
	private:

	};
}