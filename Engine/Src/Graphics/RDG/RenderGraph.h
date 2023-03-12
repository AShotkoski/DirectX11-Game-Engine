#pragma once
#include <Graphics/Graphics.h>
#include "Pass.h"
#include <vector>
#include <memory>
#include <Util/GeneralUtilities.h>

namespace Binds
{
}

class DepthStencil;
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
			DCHECK_F( finalized, "%s Attempted to execute graph that is not finalized.", name.c_str() );
			for ( auto& p : passPtrs )
			{
				p->Execute( gfx );
			}
		}	
	protected:
		RenderGraph(Graphics& gfx, const std::string graph_name );
		void AppendPass( std::unique_ptr<Pass> pPass )
		{
			passPtrs.push_back( std::move(pPass) );
			LinkSinks( *passPtrs.back() );
		}
		// Links global sink, typically called for back buffer from the final pass
		void LinkGlobalSink( const std::string& sink_name, const std::string& target_name )
		{
			const auto lam_find = [&sink_name]( const std::unique_ptr<Sink>& pSink )
			{
				return pSink->GetName() == sink_name;
			};
			const auto it = std::find_if( globalSinkPtrs.begin(), globalSinkPtrs.end(), lam_find );
			if ( it == globalSinkPtrs.end() )
			{
				ABORT_F( "Sink %s not found", sink_name.c_str() );
			}
			auto split = Util::SplitString( target_name, ".");
			DCHECK_F( split.size() == 2, "Wrong number of subsstrings generated when linking sink" );
			( *it )->SetTarget( split[0], split[1] );
		}
		
		void LinkSinks(Pass& pass)
		{
			for ( auto& s : pass.GetSinks() )
			{
				const auto& targetpass = s->GetTargetPassName();
				const auto& targetsource = s->GetTargetSourceName();
				DCHECK_F( !targetpass.empty(), "No target pass" );
				DCHECK_F( !targetsource.empty(), "No target source" );

				// Check if global target
				if ( targetpass == "$" )
				{
					bool bound = false;
					for ( auto& source : globalSourcePtrs )
					{
						if ( source->GetName() == targetsource )
						{
							s->Bind( *source );
							bound = true;
							break;
						}
					}
					if(!bound )
						ABORT_F( "Failed to bind \"%s\", no global source \"%s\" found.", s->GetName().c_str(), targetsource.c_str() );
				}
				else
				{
					bool bound = false;
					for ( auto& p : passPtrs )
					{
						if ( p->GetName() == targetpass )
						{
							s->Bind( p->GetSource( targetsource ) );
							bound = true;
							break;
						}
					}
					if(!bound )
						ABORT_F( "Couldn't find %s pass to bind", targetpass.c_str() );
				}

			}
		}
		void Finalize()
		{
			//todo validate all passes and sinks 

			finalized = true;
		}
	private:
		void RegisterGlobalSink( std::unique_ptr<Sink> pSink )
		{
			globalSinkPtrs.push_back( std::move(pSink) );
		}
		void RegisterGlobalSource( std::unique_ptr<Source> pSource )
		{
			globalSourcePtrs.push_back( std::move(pSource ));
		}
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