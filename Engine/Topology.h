#pragma once
#include "Bindable.h"

namespace Binds
{

	class Topology : public Bindable
	{
	public:
		// gfx ref isn't used, it is just necessary for the codex to function properly.
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type );
		static std::shared_ptr<Topology> Resolve( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type );
	private:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};

};