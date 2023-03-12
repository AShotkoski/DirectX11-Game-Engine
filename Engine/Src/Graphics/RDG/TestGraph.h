#pragma once
#include "RenderGraph.h"
#include "BufferClearingPass.h"

namespace RDG
{
	class TestGraph : public RenderGraph
	{
	public:
		TestGraph( Graphics& gfx )
			: RenderGraph( gfx, "TestGraph" )
		{
			{
				auto pass = std::make_unique<BufferClearingPass>( "clearRT" );
				pass->SetSinkLinkage( "buffer", "$.backbuffer" );
				AppendPass( std::move( pass ) );
			}
		}
	private:

	};
}