#pragma once
#include "Graphics.h"
#include "Loguru/loguru.hpp"
#include <type_traits>
#include <vector>
#include <optional>

namespace CB
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position_3D,
			Position_2D,
			Normal,
			Color_float_RGB,
			TexCoordUV,
			Tangent,
			Bitangent
		};

		// Template map for each elementType to keep all relations of Elementtype with concrete 
		// data types in one place.
		template<ElementType type>
		struct TypeInfo;
		template<>
		struct TypeInfo<Position_3D>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<>
		struct TypeInfo<Position_2D>
		{
			using type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<>
		struct TypeInfo<Normal>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
		};
		template<>
		struct TypeInfo<Color_float_RGB>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<>
		struct TypeInfo<TexCoordUV>
		{
			using type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
		};
		template<>
		struct TypeInfo<Tangent>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "TANGENT";
		};
		template<>
		struct TypeInfo<Bitangent>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "BITANGENT";
		};

		// Each vertex element contains its type and its offset, in bytes, into the vertexlayout
		// which can be thought of as the concrete vertex type.
		class Element
		{
		public:
			Element( ElementType type, size_t offset )
				: type( type )
				, offset( offset )
			{}
			size_t GetOffset() const
			{
				return offset;
			}
			size_t GetNextSlot() const
			{
				return offset + Size();
			}
			size_t Size() const
			{
				return SizeOfElement( type );
			}
			ElementType GetType() const
			{
				return type;
			}
			D3D11_INPUT_ELEMENT_DESC GetD3DDesc() const
			{
				switch ( type )
				{
					case Position_3D:
						return GenerateD3DDesc<Position_3D>();
					case Position_2D:
						return GenerateD3DDesc<Position_2D>();
					case Normal:
						return GenerateD3DDesc<Normal>();
					case Color_float_RGB:
						return GenerateD3DDesc<Color_float_RGB>();
					case TexCoordUV:
						return GenerateD3DDesc<TexCoordUV>();
					case Tangent:
						return GenerateD3DDesc<Tangent>();
					case Bitangent:
						return GenerateD3DDesc<Bitangent>();
				}
				assert( false && "horrible error in getd3ddesc" );
				return {};
			}
		private:
			template <ElementType ty>
			D3D11_INPUT_ELEMENT_DESC GenerateD3DDesc() const
			{
				D3D11_INPUT_ELEMENT_DESC desc = {};
				desc.AlignedByteOffset = (UINT)offset;
				desc.Format = TypeInfo<ty>::dxgiFormat;
				desc.InputSlot = 0u;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0u;
				desc.SemanticIndex = 0u;
				desc.SemanticName = TypeInfo<ty>::semantic;
				return desc;
			}
		private:
			ElementType type;
			// offset, in bytes, of this element into the vertexlayout
			size_t offset;
		};
	public:
		VertexLayout() = default;
		VertexLayout& Append( ElementType el ) noexcept
		{
			elements.emplace_back( el, elements.size() == 0u ? 0u : elements.back().GetNextSlot() );
			return *this;
		}
		size_t NumElements() const
		{
			return elements.size();
		}
		size_t SizeBytes() const
		{
			return elements.size() == 0u ? 0u : elements.back().GetNextSlot();
		}
		const Element& ResolveByIndex( size_t index ) const
		{
			assert( index < elements.size() );
			return elements[index];
		}
		// Get element with type = 'type'
		template <ElementType type>
		const Element& Resolve() const
		{
			assert( elements.size() > 0 );

			for ( const auto& e : elements )
			{
				if ( type == e.GetType() )
					return e;
			}

			assert( false && "Element not found!" );
			return elements.front();

		}
		// TODO change to template to avoid copy-paste in body
		static constexpr size_t SizeOfElement( ElementType el )
		{
			using namespace DirectX;
			// Use the kind of value that will be used to represent each element to get size
			switch ( el )
			{
				case ElementType::Position_3D:
					return sizeof( TypeInfo<Position_3D>::type );
				case ElementType::Position_2D:
					return sizeof( TypeInfo<Position_2D>::type );
				case ElementType::Color_float_RGB:
					return sizeof( TypeInfo<Color_float_RGB>::type );
				case ElementType::Normal:
					return sizeof( TypeInfo<Normal>::type );
				case ElementType::TexCoordUV:
					return sizeof( TypeInfo<TexCoordUV>::type );
				case ElementType::Tangent:
					return sizeof( TypeInfo<Tangent>::type );
				case ElementType::Bitangent:
					return sizeof( TypeInfo<Bitangent>::type );
			}
			// Invalid element was passed in, return 0;
			assert( "Invalid element passed to sizeofelement" && false );
			return 0;
		}
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DInputLayout() const
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
			// Create a input element description for each element in the layout
			for ( const auto& e : elements )
			{
				layout.push_back( e.GetD3DDesc() );
			}

			return layout;
		}
		std::string GetUID() const
		{
			std::string uid;
			for ( const auto& e : elements )
			{
				uid.append( std::to_string( e.GetType() ) );
			}
			return uid;
		}
	private:
		std::vector<Element> elements;
	};

	// Vertex view class to factor out common data to each vertex and to manage data on a 
	// per vertex basis
	class VertexView
	{
		friend class VertexBuffer;
	public:
		VertexView( char* pData, VertexLayout& layout )
			: pData( pData )
			, layout( layout )
		{
			assert( pData != nullptr );
			assert( layout.SizeBytes() > 0u );
		}
		template <typename T>
		void SetAttributeByIndex( size_t index, T&& attr )
		{
			using types = VertexLayout::ElementType;
			using namespace DirectX;
			// Get a handle to the element at index
			const auto& elem = layout.ResolveByIndex( index );

			// Figure out what kind of attribute we're dealing with
			types type = elem.GetType();
			// Get a pointer to where the attribute belongs in data
			char* pAttr = pData + elem.GetOffset();
			switch ( type )
			{
				using types = VertexLayout::ElementType;

				case types::Position_2D:
					SetAttribute<types::Position_2D>( pAttr, std::forward<T>( attr ) );
					break;
				case types::Position_3D:
					SetAttribute<types::Position_3D>( pAttr, std::forward<T>( attr ) );
					break;
				case types::Color_float_RGB:
					SetAttribute<types::Color_float_RGB>( pAttr, std::forward<T>( attr ) );
					break;
				case types::Normal:
					SetAttribute<types::Normal>( pAttr, std::forward<T>( attr ) );
					break;
				case types::TexCoordUV:
					SetAttribute<types::TexCoordUV>( pAttr, std::forward<T>( attr ) );
					break;
				case types::Tangent:
					SetAttribute<types::Tangent>( pAttr, std::forward<T>( attr ) );
					break;
				case types::Bitangent:
					SetAttribute<types::Bitangent>( pAttr, std::forward<T>( attr ) );
					break;
				default:
					assert( false && "bad element type" );
			}
		}

		template<VertexLayout::ElementType Type>
		auto& Attribute()
		{
			// Calculate the pointer to the correct data in the vertex layout
			char* pAttr = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::TypeInfo<Type>::type*>( pAttr );
		}
	private:
		template<typename Curr, typename...Trailing>
		// Unpack param list and redirect to SetAttributeByIndex
		void SetAttributeByIndex( size_t index, Curr&& attr, Trailing&&... rest )
		{
			// Call normal setAttributeByIndex
			SetAttributeByIndex( index, std::forward<Curr>( attr ) );
			// Recall this function with 1 less param, will continue being called until no more
			// trailing attributes
			SetAttributeByIndex( index + 1, std::forward<Trailing>( rest )... );
		}

		// Helper function to make code cleaner
		template<VertexLayout::ElementType DestLayoutType, typename Src >
		void SetAttribute( char* pAttr, Src&& Attr )
		{
			using Dest = typename VertexLayout::TypeInfo<DestLayoutType>::type;
			if constexpr ( std::is_assignable<Dest, Src>::value )
			{
				*reinterpret_cast<Dest*>( pAttr ) = Attr;
			}
			else
			{
				assert( false && "Non assignable passed to setattribute" );
			}
		}
	private:
		char* pData = nullptr;
		VertexLayout& layout;
	};

	// Constant version of vertex view, encapsulates a vertex object and const_casts a lot.
	class CVertexView
	{
	public:
		CVertexView( const VertexView& vertex )
			: vertex( vertex )
		{}
		template<VertexLayout::ElementType Type>
		const auto& Attribute() const
		{
			return const_cast<VertexView&>( vertex ).Attribute<Type>();
		}

	private:
		const VertexView& vertex;
	};

	// Hold the actual data of the vertices used
	class VertexBuffer
	{
	public:
		VertexBuffer( VertexLayout layout )
			: layout( layout )
		{}

		template <typename ...Args>
		// Used for emplacing vertices
		void Emplace_back( Args&&... args )
		{
			assert( sizeof...( Args ) == layout.NumElements() && "Wrong number of arguments" );
			// Resize buffer to be one vertex larger
			data.resize( data.size() + layout.SizeBytes() );
			Back().SetAttributeByIndex( 0u, std::forward<Args>( args )... );
		}
		// number of NEW verts to reserve (doesn't overload, wont downsize) 
		void Reserve( size_t nVertices )
		{
			data.resize( data.size() + ( layout.SizeBytes() * nVertices ) );
		}
		VertexView Back()
		{
			assert( data.size() >= layout.SizeBytes() );
			return VertexView( data.data() + data.size() - layout.SizeBytes(), layout );
		}
		VertexView Front()
		{
			assert( data.size() >= layout.SizeBytes() );
			return VertexView( data.data(), layout );
		}
		VertexView operator[]( size_t index )
		{
			assert( data.size() >= layout.SizeBytes() );
			assert( index < Size() );
			return VertexView( data.data() + ( layout.SizeBytes() * index ), layout );
		}
		CVertexView Back() const
		{
			return const_cast<VertexBuffer&>( *this ).Back();
		}
		CVertexView Front() const
		{
			return const_cast<VertexBuffer&>( *this ).Front();
		}
		CVertexView operator[]( size_t index ) const
		{
			return CVertexView( const_cast<VertexBuffer&>( *this )[index] );
		}
		const char* GetData() const
		{
			return data.data();
		}
		size_t SizeBytes() const
		{
			return data.size();
		}
		size_t Size() const
		{
			return data.size() == 0u ? 0u : data.size() / layout.SizeBytes();
		}
		size_t VertexSizeBytes() const
		{
			return layout.SizeBytes();
		}
		const VertexLayout& GetLayout() const
		{
			return layout;
		}
	private:
		VertexLayout layout;
		std::vector<char> data;
	};

	/******************** Dynamic Constant Buffer ************************************************/
#define SUPPORTED_TYPES \
	X(Float) \
	X(Float2) \
	X(Float3) \
	X(Float4) \
	X(Matrix) \
	X(Bool)

	// Pollute the namespace with an enum of supported types for easier access
	enum Type
	{
		#define X(el) el,
		SUPPORTED_TYPES
		#undef X
	};

	// Specialize TypeInfo struct on each supported type to use as a single truth point 
	// of System Type and Hlsl size (eg. Bool is larger in hlsl than c++)
	template<Type t>
	struct TypeInfo {};
	template<>
	struct TypeInfo<Float>
	{
		using systype = float;
		static constexpr const size_t hlslsize = sizeof( systype );
	};
	template<>
	struct TypeInfo<Float2>
	{
		using systype = DirectX::XMFLOAT2;
		static constexpr const size_t hlslsize = sizeof( systype );
	};
	template<>
	struct TypeInfo<Float3>
	{
		using systype = DirectX::XMFLOAT3;
		static constexpr const size_t hlslsize = sizeof( systype );
	};
	template<>
	struct TypeInfo<Float4>
	{
		using systype = DirectX::XMFLOAT4;
		static constexpr const size_t hlslsize = sizeof( systype );
	};
	template<>
	struct TypeInfo<Matrix>
	{
		using systype = DirectX::XMMATRIX;
		static constexpr const size_t hlslsize = sizeof( systype );
	};
	template<>
	struct TypeInfo<Bool>
	{
		using systype = bool;
		static constexpr const size_t hlslsize = sizeof( int );
	};

	// Hold a vector of composed class elements, which each have 
	class Layout
	{
		friend class View;
	private:
		// Class which holds the necessary information of each element in the buffer
		class Element
		{
		public:
			Element( Type type, const std::string& name )
				: type_( type ),
				name_( name )
			{}
			Type type() const
			{
				return type_;
			}
			const std::string& name() const
			{
				return name_;
			}
			const size_t offset() const
			{
				return *offset_;
			}
		private:
			Type type_;
			std::optional<size_t> offset_;
			std::string name_;
		};
	public:
		// Add a named element to the layout, which can later by accessed by name
		Layout& add( Type ty, const std::string& name )
		{
			DCHECK_F( !containsElement( name ), "Duplicate constant buffer element added, %s", name );
			DCHECK_F( !isAligned, "Attempted to add element to aligned layout" );
			elements_.emplace_back( ty, name );
			return *this;
		}
		const Element& QueryElementByName( const std::string& name ) const
		{
			DCHECK_F( containsElement( name ), "Attempted to query %s, but it doesn't exist.", name );
			for ( const auto& e : elements_ )
			{
				if ( name == e.name() )
					return e;
			}
		}
		bool isAligned() const
		{
			return isAligned_;
		}
	protected:
		// Get the size (in bytes) of the system type underlying a Type
		static constexpr size_t GetTypeSysSize( Type type )
		{
			switch ( type )
			{
				//case Float: return sizeof( TypeInfo<Float>::systype );
				#define X(el) case el: \
					return sizeof(TypeInfo<el>::systype);
				SUPPORTED_TYPES
					#undef X
				default:
					ABORT_F( "Unsupported type passed to GetTypeSysSize" );
			}
		}
		bool containsElement( const std::string& name ) const
		{
			// Loop elements and check name against search
			for ( const auto& e : elements_ )
			{
				if ( e.name() == name )
					return true;
			}
			return false;
		}
		void MarkAligned() { isAligned_ = true; }
	protected:
		std::vector<Element> elements_;
	private:
		bool isAligned_ = false;
	};

	/******************** Data View **************************************************************/
	class View
	{
	public:
		View( char* pData, const Layout::Element& elem)
			: pData_( pData )
			, type_( elem.type() )
		{
			pData_ += elem.offset();
		}
		template <typename T>
		void operator=( T val )
		{
			// Check data is valid
			DCHECK_F( validate<T>(), "Invalid type %s assignment.", typeid( T ).name() );
			*reinterpret_cast<T*>( pData_ ) = val;
		}

		template <typename T>
		operator T() const
		{
			DCHECK_F( validate<T>(), "Invalid type conversion %s assignment.", typeid( T ).name() );
			return *reinterpret_cast<T*>( pData_ );
		}
	private:
		template<typename T>
		bool validate( )
		{
			switch ( type_ )
			{
				#define X(el) \
				case el:\
						if(typecmp<T, el>())\
							{return true;}
				SUPPORTED_TYPES
				#undef X
				default:
					return false;
			}
		}
		template<typename T, Type Ty>
		bool typecmp() const
		{
			return typeid( T ) == typeid( TypeInfo<Ty>::systype );
		}
	private:
		char* pData_; // Points to data in buffer memory
		const Type type_; // need type for casts
	};


	/******************** Main constant buffer ***************************************************/
	class Buffer
	{
	public:
		Buffer(Layout layout)
			: layout_(layout)
		{
			// Layout.ALIGN()!
		}
		// buf["pos"] = dx::XMFLOAT3(0,1,0);
		// fthattakesXMFLOAT3(buf["pos"]) todo
		View operator[]( const std::string& name )
		{
			DCHECK_F( layout_.isAligned(), "Unaligned layout in buffer being accessed!" );
			return View( data_.data(), layout_.QueryElementByName(name));
		}
	private:
		Layout layout_;
		std::vector<char> data_;
	};
};