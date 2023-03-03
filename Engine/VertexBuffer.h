#pragma once
#include "Bindable.h"
#include "Macros.h"
#include "Vertex.h"
#include <vector>
#include <wrl.h>
#include <cassert>

namespace Binds
{

	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag );
		VertexBuffer() = default;
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const Vert::VertexBuffer& vb, std::string tag );
		static std::shared_ptr<VertexBuffer>
			Resolve( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag );
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		UINT stride;
	};


	class DynamicVertexBuffer : public VertexBuffer
	{
	public:
		DynamicVertexBuffer( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag );
		void Update(Graphics& gfx, const Vert::VertexBuffer& vb ) const;
	};

};