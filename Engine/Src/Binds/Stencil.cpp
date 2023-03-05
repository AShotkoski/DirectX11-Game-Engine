#include "Stencil.h"
#include "BindableCodex.h"

Binds::Stencil::Stencil( Graphics& gfx, Mode mode )
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{ } };

	if ( mode == Mode::Write )
	{
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if ( mode == Mode::Mask )
	{
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}

	THROW_FAILED_GFX( pGetDevice( gfx )->CreateDepthStencilState( &dsDesc, &pDepthStencilState ) );
}

void Binds::Stencil::Bind( Graphics& gfx )
{
	pGetContext( gfx )->OMSetDepthStencilState( pDepthStencilState.Get(), 0xFF );
}

std::string Binds::Stencil::GenerateUID( Mode mode )
{
	using namespace std::string_literals;
	return std::string(typeid(Stencil).name() + "_"s + std::to_string((int)mode));
}

std::shared_ptr<Binds::Stencil> Binds::Stencil::Resolve( Graphics& gfx, Mode mode )
{
	return Binds::Codex::Resolve<Stencil>(gfx, mode);
}
