#pragma once
#include <Graphics/Graphics.h>
#include "Pass.h"
#include "GraphResource.h"
#include <vector>
#include <memory>

namespace RDG
{
	class RenderGraph
	{
	public:
		RenderGraph() = default;
		const RenderGraph& operator=( const RenderGraph& ) = delete;
		RenderGraph( const RenderGraph& ) = delete;
		void RegisterExternalResource();
		void Execute();
		virtual ~RenderGraph() noexcept {};
	protected:
		//void AddPass(std::unique_ptr<Pass> pPass);
		std::shared_ptr<GraphResource> AccessOrCreateResource( const std::string& name )
		{
			return {};
		}
	private:
		//std::vector<std::unique_ptr<Pass>> pPasses_;
		std::vector<std::shared_ptr<GraphResource>> pResources_;
	};
}