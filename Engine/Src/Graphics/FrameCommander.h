#pragma once
#include "Pass.h"
#include "Job.h"
#include "Binds/BindableBaseIncludes.h"
#include "Binds/Blender.h"
#include "Binds/NullPixelShader.h"
#include <Binds/DepthStencil.h>
#include <array>

class FrameCommander
{
public:
	FrameCommander( Graphics& gfx )
		: ds( gfx, gfx.GetWidth(), gfx.GetHeight() )
	{}
	void Accept( Job job, size_t passidx )
	{
		passes[passidx].Accept( std::move( job ) );
	}
	// Script that executes passes
	void Execute(Graphics& gfx)
	{
		ds.Clear( gfx );
		gfx.BindSwapBuffer( ds );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Off )->Bind( gfx );
		passes[0].Execute( gfx );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Write )->Bind( gfx );
		Binds::NullPixelShader::Resolve( gfx )->Bind(gfx);
		passes[1].Execute( gfx );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Mask )->Bind( gfx );
		passes[2].Execute( gfx );
	}
	void Reset()
	{
		passes[0].Clear();
		passes[1].Clear();
		passes[2].Clear();
	}
private:
	std::array<Pass, 3> passes;
	DepthStencil ds;
};

