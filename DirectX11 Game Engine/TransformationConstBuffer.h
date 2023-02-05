#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include "Drawable.h"

class TransformationConstBuffer : public Bindable
{
private:
	struct TransformBuffer
	{
		DirectX::XMMATRIX model;         // Into world pos
		DirectX::XMMATRIX View; 
		DirectX::XMMATRIX Pos; 
	};
public:
	TransformationConstBuffer( Graphics& gfx, const Drawable& parent );
	void Bind( Graphics& gfx ) override;
private:
	VertexConstantBuffer<TransformBuffer> VertexCBuf;
	const Drawable& parent;
};

