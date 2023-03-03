#include "Vertex.h"

	// TODO CHANGE asserts
namespace Vert
{
	VertexLayout::Element::Element( ElementType type, size_t offset )
		: type( type )
		, offset( offset )
	{}

	size_t VertexLayout::Element::GetOffset() const
	{
		return offset;
	}

	size_t VertexLayout::Element::GetNextSlot() const
	{
		return offset + Size();
	}

	size_t VertexLayout::Element::Size() const
	{
		return SizeOfElement( type );
	}

	VertexLayout::ElementType VertexLayout::Element::GetType() const
	{
		return type;
	}

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetD3DDesc() const
	{
		switch ( type )
		{
			#define X(el)\
					case el:\
						 return GenerateD3DDesc<el>();
			VERTEX_SUPPORTED_TYPES
				#undef X
		}
		assert( false && "horrible error in getd3ddesc" );
		return { };
	}

	VertexLayout& VertexLayout::Append( ElementType el ) noexcept
	{
		elements.emplace_back( el, elements.size() == 0u ? 0u : elements.back().GetNextSlot() );
		return *this;
	}

	bool VertexLayout::Try_append( ElementType el ) noexcept
	{
		if ( !Contains( el ) )
		{
			Append( el );
			return true;
		}
		else
		{
			return false;
		}
	}

	bool VertexLayout::Contains( ElementType type ) const
	{
		for ( auto& e : elements )
		{
			if ( e.GetType() == type )
				return true;
		}
		return false;
	}

	size_t VertexLayout::NumElements() const
	{
		return elements.size();
	}

	size_t VertexLayout::SizeBytes() const
	{
		return elements.size() == 0u ? 0u : elements.back().GetNextSlot();
	}

	const VertexLayout::Element& VertexLayout::ResolveByIndex( size_t index ) const
	{
		assert( index < elements.size() );
		return elements[index];
	}

	constexpr size_t VertexLayout::SizeOfElement( ElementType el )
	{
		using namespace DirectX;
		// Use the kind of value that will be used to represent each element to get size
		switch ( el )
		{
			#define X(el)\
				case ElementType::el:\
					return sizeof(TypeInfo<el>::type);
			VERTEX_SUPPORTED_TYPES
				#undef X
		}
		// Invalid element was passed in, return 0;
		assert( "Invalid element passed to sizeofelement" && false );
		return 0;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DInputLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
		// Create a input element description for each element in the layout
		for ( const auto& e : elements )
		{
			layout.push_back( e.GetD3DDesc() );
		}

		return layout;
	}

	std::string VertexLayout::GetUID() const
	{
		std::string uid;
		for ( const auto& e : elements )
		{
			uid.append( std::to_string( e.GetType() ) );
		}
		return uid;
	}

	VertexView::VertexView( char* pData, VertexLayout& layout )
		: pData( pData )
		, layout( layout )
	{
		assert( pData != nullptr );
		assert( layout.SizeBytes() > 0u );
	}

	CVertexView::CVertexView( const VertexView& vertex )
		: vertex( vertex )
	{}

	VertexBuffer::VertexBuffer( VertexLayout layout )
		: layout( layout )
	{}

	// Populate buffer from aimesh

	VertexBuffer::VertexBuffer( VertexLayout layout, const aiMesh & mesh )
		: layout( layout )
	{
		Reserve( mesh.mNumVertices );
		for ( size_t i = 0; i < layout.NumElements(); i++ )
		{
			// Switch to get static type
			switch ( layout.ResolveByIndex( i ).GetType() )
			{
				using types = VertexLayout::ElementType;

				#define X(el)\
					case types::el:\
						SetVertsByAiMesh<types::el>(mesh);\
						break;
				VERTEX_SUPPORTED_TYPES
					#undef X
				default:
					assert( false && "bad element type" );
			}
		}
	}

	// number of NEW verts to reserve (doesn't overload, wont downsize) 

	void VertexBuffer::Reserve( size_t nVertices )
	{
		data.resize( data.size() + ( layout.SizeBytes() * nVertices ) );
	}

	VertexView VertexBuffer::Back()
	{
		assert( data.size() >= layout.SizeBytes() );
		return VertexView( data.data() + data.size() - layout.SizeBytes(), layout );
	}

	VertexView VertexBuffer::Front()
	{
		assert( data.size() >= layout.SizeBytes() );
		return VertexView( data.data(), layout );
	}

	CVertexView VertexBuffer::Back() const
	{
		return const_cast<VertexBuffer&>( *this ).Back();
	}

	CVertexView VertexBuffer::Front() const
	{
		return const_cast<VertexBuffer&>( *this ).Front();
	}

	const char* VertexBuffer::GetData() const
	{
		return data.data();
	}

	size_t VertexBuffer::SizeBytes() const
	{
		return data.size();
	}

	size_t VertexBuffer::Size() const
	{
		return data.size() == 0u ? 0u : data.size() / layout.SizeBytes();
	}

	size_t VertexBuffer::VertexSizeBytes() const
	{
		return layout.SizeBytes();
	}

	const VertexLayout& VertexBuffer::GetLayout() const
	{
		return layout;
	}

}