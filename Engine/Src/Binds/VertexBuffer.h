#pragma once
#include "Bindable.h"
#include "Macros.h"
#include <Geometry/Vertex.h>
#include <vector>
#include <wrl.h>
#include <cassert>

namespace Binds
{

	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag );	
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const Vert::VertexBuffer& vb, std::string tag );
		static std::shared_ptr<VertexBuffer>
			Resolve( Graphics& gfx, const Vert::VertexBuffer& vb, std::string tag );
	protected:
		VertexBuffer() = default;
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