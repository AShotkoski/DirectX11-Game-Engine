#include "Drawable.h"
#include <cassert>
#include "IndexBuffer.h"



void Drawable::Submit( FrameCommander& frame ) const
{

}

UINT Drawable::GetIndexCount() const
{
	return pIndexBuffer->GetIndicesCount();
}

void Drawable::AddTechnique( Technique technique )
{
	techniques.push_back( std::move(technique) );
}
