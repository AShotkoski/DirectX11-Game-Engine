#pragma once
#include "Bindable.h"

namespace Binds
{

	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx, D3D11_SAMPLER_DESC sd );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( D3D11_SAMPLER_DESC sd );
		static std::shared_ptr<Bindable> Resolve( Graphics& gfx, D3D11_SAMPLER_DESC sd );
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	};

};