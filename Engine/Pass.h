#pragma once
#include <vector>

class Pass
{
public:
	void Execute( class Graphics& gfx ) const;
	void Accept( class Job job );
	void Clear();
private:
	std::vector<class Job> jobs;
};

