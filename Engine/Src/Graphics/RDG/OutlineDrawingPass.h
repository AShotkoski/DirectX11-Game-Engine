#pragma once
#include "RenderQueuePass.h"

namespace RDG
{
	// Sinks: "depthstencil", "rendertarget"
	// Sources: "depthstencil", "rendertarget"
	class OutlineDrawingPass : public RenderQueuePass
	{
	public:
		OutlineDrawingPass( Graphics& gfx );
	};
}