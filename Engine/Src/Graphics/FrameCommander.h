#pragma once
#include "Pass.h"
#include "Job.h"
#include "Binds/BindableBaseIncludes.h"
#include "Binds/Blender.h"
#include "Binds/NullPixelShader.h"
#include <Binds/DepthStencil.h>
#include <Binds/RenderTarget.h>
#include <array>

class FrameCommander
{
public:
	FrameCommander( Graphics& gfx )
		: ds( gfx, gfx.GetWidth() , gfx.GetHeight() )
		, rt(gfx, gfx.GetWidth(), gfx.GetHeight() )
	{
		namespace dx = DirectX;
		Vert::VertexLayout lay;
		lay.Append( Vert::VertexLayout::Position_2D );
		Vert::VertexBuffer buf( std::move( lay ) );
		buf.Emplace_back( dx::XMFLOAT2{ -1,1 } );
		buf.Emplace_back( dx::XMFLOAT2{ 1,1 } );
		buf.Emplace_back( dx::XMFLOAT2{ -1,-1 } );
		buf.Emplace_back( dx::XMFLOAT2{ 1,-1 } );
		pVBFull = Binds::VertexBuffer::Resolve( gfx, buf, "$full" );
		std::vector<unsigned short> inds = { 0,1,2,1,3,2 };
		pIBFull = Binds::IndexBuffer::Resolve( gfx, inds, "$full" );

		pPSFull = Binds::PixelShader::Resolve( gfx, L"Blur_PS.cso" );
		pVSFull = Binds::VertexShader::Resolve( gfx, L"Fullscreen_VS.cso" );
		auto pByteCode = pVSFull->pGetBytecode();
		pILFull = Binds::InputLayout::Resolve( gfx, buf.GetLayout(), *pByteCode );
	}
	void Accept( Job job, size_t passidx )
	{
		passes[passidx].Accept( std::move( job ) );
	}
	// Script that executes passes
	void Execute(Graphics& gfx)
	{
		ds.Clear( gfx );
		rt.Clear( gfx );
		rt.BindAsRT( gfx, ds );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Off )->Bind( gfx );
		passes[0].Execute( gfx );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Write )->Bind( gfx );
		Binds::NullPixelShader::Resolve( gfx )->Bind(gfx);
		passes[1].Execute( gfx );

		Binds::Stencil::Resolve( gfx, Binds::Stencil::Mode::Mask )->Bind( gfx );
		passes[2].Execute( gfx );

		//fullscreen
		gfx.BindSwapBuffer( );
		rt.BindAsTex( gfx, 0 );
		pVBFull->Bind( gfx );
		pIBFull->Bind( gfx );
		pVSFull->Bind( gfx );
		pPSFull->Bind( gfx );
		pILFull->Bind( gfx );
		gfx.DrawIndexed( 6u );
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
	// fullscreen filter
	RenderTarget rt;
	std::shared_ptr<Binds::VertexBuffer> pVBFull;
	std::shared_ptr<Binds::IndexBuffer> pIBFull;
	std::shared_ptr<Binds::VertexShader> pVSFull;
	std::shared_ptr<Binds::PixelShader> pPSFull;
	std::shared_ptr<Binds::InputLayout> pILFull;
};

