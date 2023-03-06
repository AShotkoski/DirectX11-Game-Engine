#pragma once
#include "Bindable.h"

namespace Binds
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader( Graphics& gfx, const std::wstring path );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const std::wstring path );
		static std::shared_ptr<PixelShader> Resolve( Graphics& gfx, const std::wstring path );
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};

};