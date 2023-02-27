#include "Stencil.h"
#include "BindableCodex.h"

Binds::Stencil::Stencil( Graphics& gfx, Mode mode )
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{ } };

	if ( mode == Mode::Write )
	{
		dsDesc.StencilEnable = true;
		dsDesc.StencilWriteMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if ( mode == Mode::Mask )
	{
		dsDesc.StencilEnable = true;
		dsDesc.DepthEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
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

std::shared_ptr<Bindable> Binds::Stencil::Resolve( Graphics& gfx, Mode mode )
{
	return Binds::Codex::Resolve<Stencil>(gfx, mode);
}
