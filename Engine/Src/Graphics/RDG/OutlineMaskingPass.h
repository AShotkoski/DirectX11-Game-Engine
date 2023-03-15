#pragma once
#include "RenderQueuePass.h"

namespace RDG
{
	// Sinks: "depthstencil"
	// Sources: "depthstencil"
	class OutlineMaskingPass : public RenderQueuePass
	{
	public:
		OutlineMaskingPass( Graphics& gfx );
	};
}