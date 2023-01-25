#pragma once
#include "Bindable.h"
#include "ConstantBuffers.h"
#include "Drawable.h"

class TransformationConstBuffer : public Bindable
{
public:
	TransformationConstBuffer( Graphics& gfx, const Drawable& parent );
	void Bind( Graphics& gfx ) override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> VertexCBuf;
	const Drawable& parent;
};

