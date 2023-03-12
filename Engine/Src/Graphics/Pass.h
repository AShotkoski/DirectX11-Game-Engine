#pragma once
#include <vector>
#include "Graphics.h"
#include "Job.h"

class Pass
{
public:
	void Execute( Graphics& gfx ) const
	{
		for ( auto& j : jobs )
		{
			j.Execute( gfx );
		}
	}
	void Accept( Job job )
	{
		jobs.push_back( std::move( job ) );
	}
	void Clear()
	{
		jobs.clear();
	}
private:
	std::vector<Job> jobs;
};

