#include "DynamicCB.h"

namespace CB
{
	// ONLY CALLED FROM Buffer ( todo some funky inheritance to do this )

	void Layout::Align()
	{
		// Calculate boundaries of bytes of hlsl
		// jk rn its just bs

		// loop through elements and set their offset
		size_t currSize = 0u;
		for ( auto& e : elements_ )
		{
			// Set offset
			e.offset_ = currSize;
			// Calc next offset
			currSize += GetTypeSysSize( e.type() );
		}

		isAligned_ = true;
	}

	Buffer::Buffer( Layout layout )
		: layout_( layout )
	{
		layout_.Align();
		data_.resize( layout_.GetSizeBytes(), 0 );
	}
}