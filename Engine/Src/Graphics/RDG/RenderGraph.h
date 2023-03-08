#pragma once
#include <string>


namespace RDG
{
	class RenderGraph
	{
	public:
		RenderGraph();
		void RegisterExternalResource()
		virtual ~RenderGraph() noexcept {};

	private:

	};
}