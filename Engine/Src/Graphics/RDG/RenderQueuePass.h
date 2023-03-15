#pragma once
#include "BindingPass.h"
#include <Graphics/Job.h>

namespace RDG
{
	class Sink;	
	// Executes GPU commands (bindables) that come from the render queue system
	class RenderQueuePass : public BindingPass
	{
	public:
		virtual void Execute( Graphics& gfx ) override;
		void Accept( Job job );
		virtual void Clear() override;
	protected:
		RenderQueuePass( const std::string& name );
	private:
		std::vector<Job> jobs;
	};
}