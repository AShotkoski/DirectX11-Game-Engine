#include "BindingPass.h"
#include "Sink.h"
#include <Binds/Bindable.h>

namespace RDG
{
	BindingPass::BindingPass( const std::string& name )
		: Pass(name)
	{
		
	}
	void BindingPass::AddBind( std::shared_ptr<Bindable> bind )
	{
		bindPtrs.push_back( std::move( bind ) );
	}
	void BindingPass::BindAll( Graphics& gfx ) const
	{
		for ( auto& b : bindPtrs )
		{
			b->Bind( gfx );
		}
	}
}