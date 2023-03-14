#pragma once
#include "Binds/Bindable.h"
#include <vector>

namespace RDG
{
	class RenderQueuePass;
	class RenderGraph;
}

class Step
{
public:
	Step( const std::string& target_pass );
	void Submit( const class Drawable& drawable ) const;
	void Bind( Graphics& gfx ) const;
	void AddBind( std::shared_ptr<Bindable> pBind );
	void InitParentRef( const class Drawable& parent );
	void Accept( class TechniqueProbe& probe );
	void LinkTarget( RDG::RenderGraph& graph );
private:
	std::vector<std::shared_ptr<Bindable>> pBinds;
	std::string targetPass;
	RDG::RenderQueuePass* pTargetPass = nullptr;
};

