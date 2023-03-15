#pragma once

//
// This derived pass class represents any pass which needs to bind Bindables
//

#include "Pass.h"

class Bindable;
class DepthStencil;
class RenderTarget;

namespace RDG
{
	class Sink;

	class BindingPass : public Pass
	{
	protected:
		BindingPass(const std::string& name);
		void AddBind( std::shared_ptr<Bindable> bind );
		void BindAll(Graphics& gfx) const;
	private:
		void BindBuffer(Graphics& gfx) const;
	protected:
		std::shared_ptr<DepthStencil> pTargetDepthBuffer;
		std::shared_ptr<RenderTarget> pTargetBuffer;
	private:
		std::vector<std::shared_ptr<Bindable>> bindPtrs;
	};
}