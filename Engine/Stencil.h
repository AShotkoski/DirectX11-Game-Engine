#pragma once

#include "Bindable.h"
#include "Macros.h"

namespace Binds
{

	class Stencil : public Bindable
	{
	public:
		enum class Mode
		{
			Off,
			Write,
			Mask
		};
		Stencil( Graphics& gfx, Mode mode )
		{
			HRESULT hr;
			D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

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
				dsDesc.StencilReadMask = 0xFF;
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			}

			THROW_FAILED_GFX( pGetDevice( gfx )->CreateDepthStencilState( &dsDesc, &pDepthStencilState ) );
		}
		virtual void Bind( Graphics& gfx ) override
		{
			pGetContext( gfx )->OMSetDepthStencilState( pDepthStencilState.Get(), 0xFF );
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	};

}