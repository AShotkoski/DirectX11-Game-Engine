#include "Pass.h"
#include "Source.h"
#include "Sink.h"
#include <log.h>
#include <Util/GeneralUtilities.h>


namespace RDG
{
	Pass::Pass( const std::string& name )
		: name_( name )
	{}
	void Pass::SetSinkLinkage( const std::string& sink_name, const std::string& target )
	{
		// todo validate names

		auto& sink = GetSink( sink_name );
		auto split = Util::SplitString( target, "." );
		DCHECK_F( split.size() == 2, "Wrong number of subsstrings generated when linking sink" );
		sink.SetTarget( split[0], split[1] );
	}

	// Check that all sinks are linked

	bool Pass::ValidateLinkage() const
	{
		for ( const auto& s : SinkPtrs )
		{
			if ( !s->isLinked() )
				return false;
		}
		return true;
	}
	Sink& Pass::GetSink( const std::string& sink_name )
	{
		for ( auto& s : SinkPtrs )
		{
			if ( sink_name == s->GetName() )
				return *s;
		}
		ABORT_F( "\"%s\" sink not found in %s.", sink_name.c_str(), name_.c_str() );
	}
	const std::vector<std::unique_ptr<Sink>>& Pass::GetSinks() const
	{
		return SinkPtrs;
	}
	Source& Pass::GetSource( const std::string& source_name )
	{
		for ( auto& s : SourcePtrs )
		{
			if ( source_name == s->GetName() )
				return *s;
		}
		ABORT_F( "\"%s\" source not found in %s.", source_name.c_str(), name_.c_str() );
	}
	const std::string& Pass::GetName() const
	{
		return name_;
	}
	void Pass::RegisterSink( std::unique_ptr<Sink> pSink )
	{
		SinkPtrs.push_back( std::move( pSink ) );
	}
	void Pass::RegisterSource( std::unique_ptr<Source> pSource )
	{
		SourcePtrs.push_back( std::move( pSource ) );
	}
}