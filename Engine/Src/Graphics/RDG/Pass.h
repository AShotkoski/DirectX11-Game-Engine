#pragma once
#include <Graphics/Graphics.h>
#include "GraphResource.h"
#include <Graphics/BufferResource.h>
#include <string>
#include <vector>

#include "Source.h"
#include "Sink.h"
#include <log.h>

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
			// todo split target by period
			std::string split[2] = {};
			// ^^^^^^
			sink.SetTarget( split[0], split[1] );
		}
	protected:
		void RegisterSink( std::unique_ptr<Sink> pSink )
		{
			SinkPtrs.push_back( std::move( pSink ) );
		}
		void RegisterSource( std::unique_ptr<Source> pSource );
	private:
		Sink& GetSink( const std::string& sink_name )
		{
			for(auto& s : SinkPtrs )
			{
				if ( sink_name == s->GetName() )
					return *s;
			}
			ABORT_F( "Error: \"%s\" sink now found in %s.", sink_name.c_str(), name_.c_str() );
		}
	private:
		std::string name_;
		std::vector<std::unique_ptr<Sink>> SinkPtrs;
		std::vector<std::unique_ptr<Source>> SourcePtrs;
	};

}