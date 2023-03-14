#include "RenderGraph.h"
#include "Sink.h"
#include "Source.h"
#include <Graphics/BufferResource.h>
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>
#include <Util/GeneralUtilities.h>
#include "Pass.h"
#include "RenderQueuePass.h"

namespace RDG
{

	void RenderGraph::Execute( Graphics& gfx )
	{
		DCHECK_F( finalized, "%s Attempted to execute graph that is not finalized.", name.c_str() );
		for ( auto& p : passPtrs )
		{
			p->Execute( gfx );
		}
	}


	RenderQueuePass& RenderGraph::GetRenderQueue( const std::string& pass_name )
	{
		for ( auto& p : passPtrs )
		{
			if ( p->GetName() == pass_name ) // todo  type check && std::is_same<RenderQueuePass, decltype( *(p.get()) )>::value
				return *static_cast<RenderQueuePass*>(p.get());
		}
		ABORT_F( "%s not found", pass_name.c_str() );
	}

	void RenderGraph::Clear()
	{
		for ( auto& p : passPtrs )
		{
			p->Clear();
		}
	}

	RenderGraph::RenderGraph( Graphics& gfx, const std::string graph_name )
		: name( graph_name )
		, targetBuffer( gfx.pGetRenderTarget() )
		, masterDS( std::make_shared<DepthStencil>( gfx, gfx.GetWidth(), gfx.GetHeight() ) )
	{
		// Register globals
		RegisterGlobalSink( BufferSink<RenderTarget>::MakeUnique( "backbuffer", targetBuffer ) );
		RegisterGlobalSource( BufferSource<RenderTarget>::MakeUnique( "backbuffer", targetBuffer ) );
		RegisterGlobalSource( BufferSource<DepthStencil>::MakeUnique( "depthstencil", masterDS ) );
	}

	void RenderGraph::AppendPass( std::unique_ptr<Pass> pPass )
	{
		passPtrs.push_back( std::move( pPass ) );
		LinkSinks( *passPtrs.back() );
	}

	// Links global sink, typically called for back buffer from the final pass

	void RenderGraph::LinkGlobalSink( const std::string& sink_name, const std::string& target_name )
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
		auto split = Util::SplitString( target_name, "." );
		DCHECK_F( split.size() == 2, "Wrong number of subsstrings generated when linking sink" );
		( *it )->SetTarget( split[0], split[1] );

		// Bind
		bool bound = false;
		for ( auto& p : passPtrs )
		{
			if ( p->GetName() == (*it)->GetTargetPassName() )
			{
				(*it)->Bind( p->GetSource( (*it)->GetTargetSourceName() ) );
				bound = true;
				break;
			}
		}
		if ( !bound )
			ABORT_F( "Couldn't find %s pass to bind", ( *it )->GetTargetPassName().c_str() );
	}

	void RenderGraph::LinkSinks( Pass& pass )
	{
		for ( auto& s : pass.GetSinks() )
		{
			const auto& targetpass = s->GetTargetPassName();
			const auto& targetsource = s->GetTargetSourceName();
			DCHECK_F( !targetpass.empty(), "Missing target pass on sink \"%s\"", s->GetName().c_str() );
			DCHECK_F( !targetsource.empty(), "Missing target source on sink \"%s\"", s->GetName().c_str() );

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
				if ( !bound )
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
				if ( !bound )
					ABORT_F( "Couldn't find %s pass to bind", targetpass.c_str() );
			}
			// Log bind
			DLOG_F( INFO, "Graph \"%s,\" bound source \"%s\" on pass \"%s\" to sink \"%s\" on pass \"%s.\"",
					name.c_str(), targetsource.c_str(), targetpass.c_str(), s->GetName().c_str(), pass.GetName().c_str() );
		}
	}

	void RenderGraph::Finalize()
	{
		for ( const auto& p : passPtrs )
		{
			if ( !p->ValidateLinkage() )
			{
				// todo exception
				ABORT_F( "Linkage validation failed on \"%s\"", name.c_str() );
			}
		}
		for ( const auto& s : globalSinkPtrs )
		{
			if ( !s->isLinked() )
			{
				ABORT_F( "Linkage validation failed on \"%s\"", name.c_str() );
			}
		}
		finalized = true;
	}

	void RenderGraph::RegisterGlobalSink( std::unique_ptr<Sink> pSink )
	{
		globalSinkPtrs.push_back( std::move( pSink ) );
	}

	void RenderGraph::RegisterGlobalSource( std::unique_ptr<Source> pSource )
	{
		globalSourcePtrs.push_back( std::move( pSource ) );
	}

}