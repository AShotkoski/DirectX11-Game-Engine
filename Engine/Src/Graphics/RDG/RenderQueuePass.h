#pragma once
#include "BindingPass.h"

class Job;
class RenderTarget;
class DepthStencil;

namespace RDG
{
	class Sink;
	// Sinks:	"rendertarget", "depthstencil"
	// Sources: "rendertarget", "depthstencil"
	// Executes GPU commands (bindables) that come from the render queue system
	class RenderQueuePass : public BindingPass
	{
	public:
		RenderQueuePass( const std::string& name );
		virtual void Execute( Graphics& gfx ) override;
		void Accept( Job job );
		virtual void Clear() override;
	private:
		std::vector<Job> jobs;
		std::shared_ptr<RenderTarget> pTargetBuffer;
		std::shared_ptr<DepthStencil> pTargetDepthBuffer;
	};
}