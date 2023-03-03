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
		Stencil( Graphics& gfx, Mode mode );
		virtual void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( Mode mode );
		static std::shared_ptr<Stencil> Resolve( Graphics& gfx, Mode mode );
	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	};

}