#pragma once
#include "BindingPass.h"

class Job;

namespace RDG
{
	class RenderQueuePass : public BindingPass
	{
	public:
		RenderQueuePass( const std::string& name );
		virtual void Execute( Graphics& gfx ) override;
		void Accept( Job job );
		void Clear();
	private:
		std::vector<Job> jobs;
	};
}