#pragma once
#include <Graphics/Graphics.h>
#include "GraphResource.h"
#include <Graphics/BufferResource.h>
#include <string>
#include <vector>

#include "Source.h"
#include "Sink.h"
#include <log.h>
#include <Util/GeneralUtilities.h>

namespace RDG
{
	class Pass
	{
	public:
		Pass( const std::string& name )
			: name_( name )
		{}
		virtual void Execute(Graphics& gfx) = 0;
		virtual ~Pass() = default;
		// Target is target pass and target source separated by a period eg "lambertian.gbuffer"
		virtual void SetSinkLinkage( const std::string& sink_name, const std::string& target )
		{
			// todo validate names

			auto& sink = GetSink( sink_name );
			auto split = Util::SplitString( target, "." );
			DCHECK_F( split.size() == 2, "Wrong number of subsstrings generated when linking sink" );
			sink.SetTarget( split[0], split[1] );
		}
		Sink& GetSink( const std::string& sink_name )
		{
			for(auto& s : SinkPtrs )
			{
				if ( sink_name == s->GetName() )
					return *s;
			}
			ABORT_F( "\"%s\" sink not found in %s.", sink_name.c_str(), name_.c_str() );
		}
		const std::vector<std::unique_ptr<Sink>>& GetSinks() const
		{
			return SinkPtrs;
		}
		Source& GetSource( const std::string& source_name )
		{
			for(auto& s : SourcePtrs )
			{
				if ( source_name == s->GetName() )
					return *s;
			}
			ABORT_F( "\"%s\" source not found in %s.", source_name.c_str(), name_.c_str() );
		}
		const std::string& GetName() const
		{
			return name_;
		}
	protected:
		void RegisterSink( std::unique_ptr<Sink> pSink )
		{
			SinkPtrs.push_back( std::move( pSink ) );
		}
		void RegisterSource( std::unique_ptr<Source> pSource )
		{
			SourcePtrs.push_back( std::move( pSource ) );
		}
	private:
		std::string name_;
		std::vector<std::unique_ptr<Sink>> SinkPtrs;
		std::vector<std::unique_ptr<Source>> SourcePtrs;
	};

}