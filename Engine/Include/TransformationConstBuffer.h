#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"

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
		TransformationConstBuffer( Graphics& gfx);
		virtual void Bind( Graphics& gfx ) override;
		void InitParentRefs( const class Drawable& parent ) override;
		// No Codex things because a transform cbuf is by nature going to be unique so sharing doesn't make sense
	protected:
		TransformBuffer GetBuffer(Graphics& gfx) const;
		void UpdateAndBind( Graphics& gfx, TransformBuffer buffer ) const;
	private:
		static std::unique_ptr<VertexConstantBuffer<TransformBuffer>> pVertexCBuf;
		const class Drawable* pParent;
	};

};