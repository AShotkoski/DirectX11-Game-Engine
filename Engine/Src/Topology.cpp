#include "Topology.h"
#include "BindableCodex.h"
#include <typeinfo>

namespace Binds
{

	Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type )
		:
		type( type )
	{}

	void Topology::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->IASetPrimitiveTopology( type );
	}

	std::string Topology::GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type )
	{
		using namespace std::string_literals;
		return std::string(typeid(Topology).name()) + "_"s + std::to_string(type);
	}

	std::shared_ptr<Topology> Topology::Resolve( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type )
	{
		return Codex::Resolve<Topology>(gfx, type);
	}

};