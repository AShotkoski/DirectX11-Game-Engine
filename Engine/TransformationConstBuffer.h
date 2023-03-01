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
		virtual void Bind( Graphics& gfx ) override;
		static std::string GenerateUID( const Drawable& parent );
		static std::shared_ptr<TransformationConstBuffer> Resolve( Graphics& gfx, const Drawable& parent );
	protected:
		TransformBuffer GetBuffer(Graphics& gfx) const;
		void UpdateAndBind( Graphics& gfx, TransformBuffer buffer ) const;
	private:
		static std::unique_ptr<VertexConstantBuffer<TransformBuffer>> pVertexCBuf;
		const Drawable& parent;
	};

};