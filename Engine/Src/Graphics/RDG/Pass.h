#pragma once
#include <Graphics/Graphics.h>
#include "GraphResource.h"
#include <string>
#include <functional>

namespace RDG
{
	class Pass
	{
	public:
		Pass( const std::string& name )
			: name_( name )
		{}
		virtual void Execute();
		virtual ~Pass() = default;
	private:
		std::string name_;
	};

}