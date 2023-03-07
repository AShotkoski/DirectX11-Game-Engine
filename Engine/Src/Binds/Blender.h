#pragma once
#include "Bindable.h"

namespace Binds
{
	class Blender : public Bindable
	{
	public:
		Blender( Graphics& gfx, bool blendEnabled = false );
		virtual void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( bool blendEnabled );
		static std::shared_ptr<Blender> Resolve( Graphics& gfx, bool blendEnabled = false );
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
		bool isBlending;
	};
}// ns binds