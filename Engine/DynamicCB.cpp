#include "DynamicCB.h"

namespace CB
{
	// ONLY CALLED FROM Buffer ( todo some funky inheritance to do this )

	void Layout::Align()
	{
		// Calculate boundaries of bytes of hlsl

		// loop through elements and set their offset
		size_t totalSize = 0u;
		for ( auto& e : elements_ )
		{
			// Set offset
			// Calculate next offset ( total size )
			auto distFromBoundary = totalSize % 16;
			auto currentSize = GetTypeSysSize( e.type() );
			if ( distFromBoundary + currentSize > 16 )
			{
				// On boundary, add padding
				totalSize += 16 - distFromBoundary;
			}
			e.offset_ = totalSize;
			totalSize += currentSize;
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