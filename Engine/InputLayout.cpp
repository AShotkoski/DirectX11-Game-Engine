#include "InputLayout.h"
#include "Macros.h"
#include "BindableCodex.h"
#include <cassert>

namespace Binds
{

	InputLayout::InputLayout( Graphics& gfx,
							  const Vert::VertexLayout& layout,
							  ID3DBlob& VSByteCode )
	{
		// error handling
		HRESULT hr;
		assert( layout.NumElements() > 0);

		const auto descriptions = layout.GetD3DInputLayout();

		THROW_FAILED_GFX( pGetDevice( gfx )->CreateInputLayout(
			descriptions.data(),
			(UINT)descriptions.size(),
			VSByteCode.GetBufferPointer(),
			VSByteCode.GetBufferSize(),
			&pInputLayout ) );
	}

	void InputLayout::Bind( Graphics& gfx )
	{
		pGetContext( gfx )->IASetInputLayout( pInputLayout.Get() );
	}

	std::string InputLayout::GenerateUID( const Vert::VertexLayout& layout, ID3DBlob& )
	{
		using namespace std::string_literals;
		return std::string(typeid(InputLayout).name() + "_"s + layout.GetUID());
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve( Graphics& gfx, const Vert::VertexLayout& layout, ID3DBlob& VSByteCode )
	{
		return Codex::Resolve<InputLayout>(gfx, layout, VSByteCode);
	}

};