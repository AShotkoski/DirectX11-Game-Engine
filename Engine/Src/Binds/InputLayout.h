#pragma once
#include "Bindable.h"
#include <Geometry/Vertex.h>
#include <vector>

namespace Binds
{

	class InputLayout : public Bindable
	{
	public:
		InputLayout( Graphics& gfx, const Vert::VertexLayout& layout,
					 ID3DBlob& VSByteCode );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const Vert::VertexLayout& layout, ID3DBlob&);
		static std::shared_ptr<InputLayout> Resolve( Graphics& gfx, const Vert::VertexLayout& layout,
												  ID3DBlob& VSByteCode );
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};

};