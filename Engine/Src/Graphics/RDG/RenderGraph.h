#pragma once
#include <Graphics/Graphics.h>
#include "Pass.h"
#include <vector>
#include <memory>

namespace Binds
{
	class DepthStencil;
}

class RenderTarget;

namespace RDG
{
	class Source;
	class Sink;

	class RenderGraph
	{
	public:
		
		const RenderGraph& operator=( const RenderGraph& ) = delete;
		RenderGraph( const RenderGraph& ) = delete;
		// todo: void RegisterExternalResource();
		virtual ~RenderGraph() noexcept {};
		void Execute(Graphics& gfx)
		{
			for ( auto& p : passPtrs )
			{
				p->Execute( gfx );
			}
		}
		
	protected:
		RenderGraph(Graphics& gfx, const std::string graph_name );
		std::shared_ptr<GraphResource> AccessOrCreateResource( const std::string& name )
		{
			return {};
		}
		void AppendPass( std::unique_ptr<Pass>&& pPass )
		{
			passPtrs.push_back( pPass );
		}
	private:
		void RegisterGlobalSink( std::unique_ptr<Sink>&& pSink )
		{
			globalSinkPtrs.push_back( pSink );
		}
		void RegisterGlobalSource( std::unique_ptr<Source>&& pSource )
		{
			globalSourcePtrs.push_back( pSource );
		}
	protected:
		std::shared_ptr<RenderTarget> targetBuffer;
		std::shared_ptr<Binds::DepthStencil> masterDS;
	private:
		std::string name;
		std::vector<std::unique_ptr<Pass>> passPtrs;
		std::vector<std::unique_ptr<Source>> globalSourcePtrs;
		std::vector<std::unique_ptr<Sink>> globalSinkPtrs;
	}; 
}