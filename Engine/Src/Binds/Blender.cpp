#include "Blender.h"
#include "Macros.h"
#include "BindableCodex.h"

namespace Binds
{
	Blender::Blender( Graphics& gfx, bool blendEnabled )
		: isBlending(blendEnabled)
	{
		HRESULT hr;

		CD3D11_BLEND_DESC bdesc{ CD3D11_DEFAULT{} };
		if ( isBlending )
		{
			bdesc.RenderTarget->BlendEnable = true;
			bdesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA;
			bdesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		else
		{
			
		}
		THROW_FAILED_GFX( pGetDevice( gfx )->CreateBlendState( &bdesc, &pBlendState ) );
	}
	void Blender::Bind( Graphics& gfx )
	{
		if ( isBlending )
		{
			pGetContext( gfx )->OMSetBlendState( pBlendState.Get(), nullptr, 0xFFFFFFFF );
		}
		else
		{
			pGetContext( gfx )->OMSetBlendState( nullptr, nullptr, 0xFFFFFFFF );
		}
	}
	std::string Blender::GenerateUID( bool blendEnabled )
	{
		return std::string(typeid(Blender).name() + std::string("_") + std::to_string((int)blendEnabled));
	}
	std::shared_ptr<Blender> Blender::Resolve( Graphics& gfx, bool blendEnabled )
	{
		return Codex::Resolve<Blender>(gfx, blendEnabled);
	}
}
