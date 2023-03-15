#include "BindingPass.h"
#include "Sink.h"
#include <Binds/Bindable.h>
#include <Binds/RenderTarget.h>
#include <Binds/DepthStencil.h>

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
		BindBuffer(gfx);
		for ( auto& b : bindPtrs )
		{
			b->Bind( gfx );
		}
	}
	void BindingPass::BindBuffer(Graphics& gfx) const
	{
		if ( !pTargetBuffer && !pTargetDepthBuffer )
			ABORT_F( "No target buffer or depth stencil is initialized in %s", GetName().c_str() );
		if ( pTargetBuffer )
		{
			if ( pTargetDepthBuffer ) // has render target and depth/stencil
				pTargetBuffer->Bind( gfx, *pTargetDepthBuffer );
			else // has only render target
				pTargetBuffer->Bind( gfx );
		}
		else // has only depth stencil
		{
			pTargetDepthBuffer->Bind( gfx );
		}
	}
}