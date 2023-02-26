#include "DynamicCB.h"

namespace CB
{
	// ONLY CALLED FROM Buffer ( todo some funky inheritance to do this )

	Layout::Layout() : isAligned_( false ) {}

	// Add a named element to the layout, which can later by accessed by name

	Layout& Layout::add( Type ty, const std::string& name )
	{
		DCHECK_F(
			!containsElement( name ),
			"Duplicate constant buffer element added, %s",
			name.c_str() );
		DCHECK_F( !isAligned(), "Attempted to add element to aligned layout" );
		elements_.emplace_back( ty, name );
		return *this;
	}

	const Layout::Element& Layout::QueryElementByName( const std::string& name ) const
	{
		DCHECK_F(
			containsElement( name ),
			"Attempted to query %s, but it doesn't exist.",
			name.c_str() );
		for( const auto& e : elements_ )
		{
			if( name == e.GetName() )
				return e;
		}
		ABORT_F( "Queried invalid element." );
		// Never called
		return elements_.front();
	}

	bool Layout::isAligned() const
	{
		return isAligned_;
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

		isAligned_ = true;
	}

	size_t Layout::GetSizeBytes() const
	{
		DCHECK_F( isAligned(), "Attempted to get size of non aligned layout." );
		return elements_.back().GetOffset() + GetTypeSysSize( elements_.back().GetType() );
	}

	// Get the size (in bytes) of the system type underlying a Type

	constexpr size_t Layout::GetTypeSysSize( Type type )
	{
		switch( type )
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

	bool Layout::containsElement( const std::string& name ) const
	{
		// Loop elements and check name against search
		for( const auto& e : elements_ )
		{
			if( e.GetName() == name )
				return true;
		}
		return false;
	}

	Buffer::Buffer( Layout layout ) : layout_( layout )
	{
		layout_.Align();
		data_.resize( layout_.GetSizeBytes(), 0 );
	}
	size_t Buffer::sizeBytes() const
	{
		return layout_.GetSizeBytes();
	}
	Layout::Element::Element( Type type, const std::string& name ) : type_( type ), name_( name ) {}
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

	View Buffer::operator[]( const std::string& name )
	{
		DCHECK_F( layout_.isAligned(), "Unaligned layout in buffer being accessed!" );
		return View( data_.data(), layout_.QueryElementByName( name ) );
	}

	View::View( char* pData, const Layout::Element& elem )
		: pData_( pData )
		, type_( elem.GetType() )
	{
		pData_ += elem.GetOffset();
	}
}