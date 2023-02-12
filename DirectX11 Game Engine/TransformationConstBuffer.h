#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include "Drawable.h"

namespace Binds
{

	class TransformationConstBuffer : public Bindable
	{
	private:
		struct TransformBuffer
		{
			DirectX::XMMATRIX model;         // Into world pos
			DirectX::XMMATRIX InvView;
			DirectX::XMMATRIX modelViewProj;
		};
	public:
		TransformationConstBuffer( Graphics& gfx, const Drawable& parent );
		void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const Drawable& parent );
		static std::shared_ptr<Bindable> Resolve( Graphics& gfx, const Drawable& parent );
	private:
		VertexConstantBuffer<TransformBuffer> VertexCBuf;
		const Drawable& parent;
	};

};