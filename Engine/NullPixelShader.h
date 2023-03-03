#pragma once
#include "Bindable.h"

namespace Binds
{

	class NullPixelShader : public Bindable
	{
	public:
		NullPixelShader( Graphics& gfx );
		virtual void Bind( Graphics& gfx ) override;
		static std::string GenerateUID();
		static std::shared_ptr<NullPixelShader> Resolve( Graphics& gfx );
	};

}