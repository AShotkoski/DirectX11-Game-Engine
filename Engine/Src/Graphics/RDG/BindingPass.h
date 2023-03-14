#pragma once

//
// This derived pass class represents any pass which needs to bind Bindables
//

#include "Pass.h"

class Bindable;

namespace RDG
{
	class BindingPass : public Pass
	{
	protected:
		BindingPass(const std::string& name);
		void AddBind( std::shared_ptr<Bindable> bind );
		void BindAll(Graphics& gfx) const;
	private:
		std::vector<std::shared_ptr<Bindable>> bindPtrs;
	};
}