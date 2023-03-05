#pragma once
#include "Bindable.h"

namespace Binds
{
	// todo: better errors when file load file (also this class is just yucky)
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