#pragma once
#include "Bindable.h"

namespace Binds
{

	class Sampler : public Bindable
	{
	public:
		Sampler( Graphics& gfx );
		void Bind( Graphics& gfx ) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
	};

};