#include "DynamicCB.h"

namespace CB
{
	/******************** Layout Element *********************************************************/

	Layout::Element::Element( Type type, const std::string& name )
		: type_( type )
		, name_( name )
	{}
	Type Layout::Element::GetType() const
	{
		return type_;
	}
	const std::string& Layout::Element::GetName() const
	{
		return name_;
	}
	const size_t Layout::Element::GetOffset() const
	{
		return *offset_;
	}

	/******************** Layout *****************************************************************/

	// Add a named element to the layout, which can later by accessed by name
	Layout& Layout::add( Type ty, const std::string& name )
	{
		DCHECK_F(
			!ContainsElement( name ),
			"Duplicate constant buffer element added, use try_add %s",
			name.c_str() );
		elements_.emplace_back( ty, name );
		return *this;
	}

	bool Layout::try_add( Type ty, const std::string& name )
	{
		if ( !ContainsElement( name ) )
		{
			add( ty, name );
			return true;
		}
		else 
		{
			return false;
		}
	}

	constexpr size_t Layout::GetTypeSysSize( Type type )
	{
		switch ( type )
		{
			#define X( el ) \
				case el: \
					return sizeof( TypeInfo<el>::systype );
			SUPPORTED_TYPES
				#undef X
			default:
				ABORT_F( "Unsupported type passed to GetTypeSysSize" );
		}
	}

	bool Layout::ContainsElement( const std::string& name ) const
	{
		// Loop elements and check name against search
		for ( const auto& e : elements_ )
		{
			if ( e.GetName() == name )
				return true;
		}
		return false;
	}

	const Layout::Element& Layout::QueryElementByName( const std::string& name ) const
	{
		DCHECK_F(
			ContainsElement( name ),
			"Attempted to query %s, but it doesn't exist.",
			name.c_str() );

		for( const auto& e : elements_ )
		{
			if( name == e.GetName() )
				return e;
		}
		ABORT_F( "Queried invalid element." );
	}

	void Layout::Align()
	{
		// Calculate boundaries of bytes of hlsl

		// loop through elements and set their offset
		size_t totalSize = 0u;
		for( auto& e : elements_ )
		{
			// Set offset
			// Calculate next offset ( total size )
			auto distFromBoundary = totalSize % 16;
			auto currentSize      = GetTypeSysSize( e.GetType() );
			if( distFromBoundary + currentSize > 16 )
			{
				// On boundary, add padding
				totalSize += 16 - distFromBoundary;
			}
			e.offset_ = totalSize;
			totalSize += currentSize;
		}
	}

	size_t Layout::GetSizeBytes() const
	{
		return elements_.back().GetOffset() + GetTypeSysSize( elements_.back().GetType() );
	}

	Type Layout::GetTypeAt( const std::string& name ) const
	{
		return QueryElementByName( name ).GetType();
	}

	size_t Layout::GetOffsetAt( const std::string& name ) const
	{
		return QueryElementByName( name ).GetOffset();
	}

	/******************** Aligned Layout *********************************************************/

	AlignedLayout::AlignedLayout( Layout unaligned )
		: Layout( std::move( unaligned ) )
	{
		Align();
	}

	size_t AlignedLayout::GetSizeBytes() const
	{
		// Use parent function to get size so we can keep elements encapsulated
		return Layout::GetSizeBytes();
	}

	bool AlignedLayout::Contains( const std::string name ) const
	{
		return Layout::ContainsElement(name);
	}

	/******************** Views ******************************************************************/

	View::View( char* pData, const AlignedLayout& layout, const std::string& name )
		: pData_( pData )
		, type_( layout.GetTypeAt( name ) )
	{
		
		pData_ += layout.GetOffsetAt(name);
	}

	/******************** Buffer *****************************************************************/

	Buffer::Buffer( Layout layout )
		: layout_( std::move(layout) )
	{
		// Set size of data buffer to match size of layout
		data_.resize( layout_.GetSizeBytes(), 0 );
	}

	size_t Buffer::sizeBytes() const
	{
		return layout_.GetSizeBytes();
	}
	const char* Buffer::GetData() const
	{
		return data_.data();
	}


	View Buffer::operator[]( const std::string& name )
	{
		DCHECK_F( layout_.Contains( name ), "Attempted to get view on non-existant element %s", name.c_str() );
		return View( data_.data(), layout_, name );
	}

}