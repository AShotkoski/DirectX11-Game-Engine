#pragma once

#include "Pass.h"
#include "Sink.h"
#include "Source.h"

namespace RDG
{
	// Sinks:	"buffer"
	// Sources: "buffer"
	// Clears Buffer
	class BufferClearingPass : public Pass
	{
	public:
		BufferClearingPass( const std::string& name )
			: Pass(name)
		{
			RegisterSink( BufferSink<BufferResource>::MakeUnique( "buffer", pBuffer ) );
			RegisterSource( BufferSource<BufferResource>::MakeUnique( "buffer", pBuffer ) );
		}
		virtual void Execute( Graphics& gfx ) override
		{
			pBuffer->Clear( gfx );
		}
	private:
		std::shared_ptr<BufferResource> pBuffer;
	};
}