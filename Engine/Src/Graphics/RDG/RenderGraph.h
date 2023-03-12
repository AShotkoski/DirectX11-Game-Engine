#pragma once
#include <Graphics/Graphics.h>
#include <vector>
#include <memory>


class DepthStencil;
class RenderTarget;

namespace RDG
{
	class Source;
	class Sink;
	class Pass;

	class RenderGraph
	{
	public:	
		const RenderGraph& operator=( const RenderGraph& ) = delete;
		RenderGraph( const RenderGraph& ) = delete;
		virtual ~RenderGraph() noexcept = default;
		void Execute( Graphics& gfx );
	protected:
		RenderGraph(Graphics& gfx, const std::string graph_name );
		void AppendPass( std::unique_ptr<Pass> pPass );
		// Links global sink, typically called for back buffer from the final pass
		void LinkGlobalSink( const std::string& sink_name, const std::string& target_name );	
		void LinkSinks( Pass& pass );
		// Validates all sinks on every pass and self are linked
		void Finalize();
	private:
		void RegisterGlobalSink( std::unique_ptr<Sink> pSink );
		void RegisterGlobalSource( std::unique_ptr<Source> pSource );
	protected:
		std::shared_ptr<RenderTarget> targetBuffer;
		std::shared_ptr<DepthStencil> masterDS;
	private:
		bool finalized = false;
		std::string name;
		std::vector<std::unique_ptr<Pass>> passPtrs;
		std::vector<std::unique_ptr<Source>> globalSourcePtrs;
		std::vector<std::unique_ptr<Sink>> globalSinkPtrs;
	}; 
}