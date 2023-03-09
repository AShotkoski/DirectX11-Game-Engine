#pragma once
#include "RenderGraph.h"
#include "Pass.h"

namespace RDG
{
	class OutlinedLambertGraph : public RenderGraph
	{
	public:
		OutlinedLambertGraph(Graphics& gfx)
		{
			// add lambertian pass
			{
				struct Lamb
				{
					std::vector<std::shared_ptr<GraphResource>> Sinks;
					std::vector<std::shared_ptr<GraphResource>> Sources;
				};
				Lamb passParams
				{
					{AccessOrCreateResource("g_DS"), AccessOrCreateResource( "g_BackBuffer" ) },
					{AccessOrCreateResource( "g_DS" ), AccessOrCreateResource( "g_BackBuffer" ) }
				};
				Pass p( "Lambertian", passParams, [passParams]() { } );

			}
		}
	private:
	};
}