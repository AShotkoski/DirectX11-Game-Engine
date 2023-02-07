#pragma once
#include "Bindable.h"

namespace Binds
{

	class PixelShader : public Bindable
	{
	public:
		PixelShader( Graphics& gfx, const std::wstring path );
		void Bind( Graphics& gfx ) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};

};