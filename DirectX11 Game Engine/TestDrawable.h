#pragma once

#include "Drawable.h"
#include "Topology.h"
#include "VertexBuffer.h"

#include <DirectXMath.h>

class tDrawable : public Drawable
{
public:
	tDrawable()
	{
		AddBind( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	}
	void Update( float dt ) override
	{
		
	}
};