#pragma once
#include "RenderQueuePass.h"

namespace RDG
{
	// Sinks:	"rendertarget", "depthstencil"
	// Sources: "rendertarget", "depthstencil"
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass( Graphics& gfx, const std::string& name );
	private:
	};
}