#pragma once
#include "Bindable.h"

namespace Binds
{

	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID(  );
		static std::shared_ptr<Bindable> Resolve( Graphics& gfx );
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	};

};