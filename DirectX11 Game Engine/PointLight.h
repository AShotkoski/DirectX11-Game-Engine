#pragma once
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight( Graphics& gfx );
	void Bind( Graphics& gfx ) const;
	void SpawnControlWindow();
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 LightPos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 LightPos;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};

