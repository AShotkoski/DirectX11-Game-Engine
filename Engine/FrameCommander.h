#pragma once
#include "Pass.h"
#include "Job.h"
#include "BindableBaseIncludes.h"
#include <array>

class FrameCommander
{
public:
	void Accept( Job job, size_t passidx )
	{
		passes[passidx].Accept( std::move( job ) );
	}
	// Script that executes passes
	void Execute(Graphics& gfx)
	{
		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Off )->Bind( gfx );
		passes[0].Execute( gfx );
	}
	void Reset()
	{
		passes[0].Clear();
		passes[1].Clear();
		passes[2].Clear();
	}
private:
	std::array<Pass, 3> passes;
};

