#include "Step.h"

Step::Step( size_t passNum )
	: passIdx(passNum)
{
}

void Step::AddBind( std::shared_ptr<Bindable> pBind )
{
	pBinds.push_back( std::move( pBind ) );
}
