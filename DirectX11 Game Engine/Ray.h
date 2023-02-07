#pragma once

#include "Drawable.h"
#include "BindableBaseIncludes.h"
#include "Vertex.h"

class Ray : public DrawableBase<Ray>
{
public:
	Ray( Graphics& gfx, float length = 25.f, DirectX::XMFLOAT3 source = {0,0,0 },
		 DirectX::XMFLOAT3 dir = {0,0,0 });
	void SetDir( Graphics& gfx, DirectX::XMFLOAT3 dir );
	void SetOrigin( Graphics& gfx, DirectX::XMFLOAT3 origin );
	virtual void Update( float dt ) override
	{}
	virtual DirectX::XMMATRIX GetTransformationMatrix() const noexcept override;
	// override draw function to call drawindexed
	virtual void Draw( Graphics& gfx ) const override;
private:
	// Lazy helper class to set direction vector 
	void SetDirHelper();
private:
	Vert::VertexBuffer vertbuf;
	Binds::DynamicVertexBuffer* dynamicVB = nullptr;
	float len;
};