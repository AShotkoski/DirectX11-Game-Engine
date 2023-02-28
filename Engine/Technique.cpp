#include "Technique.h"

void Technique::AddStep( Step step )
{
	steps.push_back( std::move( step ) );
}
