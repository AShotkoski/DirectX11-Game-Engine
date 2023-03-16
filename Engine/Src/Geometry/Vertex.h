#pragma once
// Vertex Layout class holds a list of Elements which describe the layout of a vertex,
// in order to describe a useful vertex layout we must also keep track of the order of the 
// vertex elements.
#include <Graphics/Graphics.h>
#include <type_traits>
#include <vector>
#include <assimp/mesh.h>


namespace Vert
{

	#define VERTEX_SUPPORTED_TYPES \
		X(Position_3D) \
		X(Position_2D) \
		X(Normal) \
		X(Color_float_RGB) \
		X(TexCoordUV) \
		X(Tangent) \
		X(Bitangent)

	class VertexLayout
	{
	public:
		enum ElementType
		{
			#define X(el) el,
			VERTEX_SUPPORTED_TYPES
			#undef X
		};

		#define EXTRA_AI_MESH_VERT_ELEM(aitype) static type Extract(const aiMesh& mesh, size_t i) {return *reinterpret_cast<const type*>(&mesh.aitype[i]);}

		// Template map for each elementType to keep all relations of Elementtype with concrete 
		// data types in one place.
		template<ElementType type>
		struct TypeInfo;
		template<> struct TypeInfo<Position_3D>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			EXTRA_AI_MESH_VERT_ELEM( mVertices )
		};
		template<> struct TypeInfo<Position_2D>
		{
			using type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
			EXTRA_AI_MESH_VERT_ELEM( mVertices )
		};
		template<> struct TypeInfo<Normal>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			EXTRA_AI_MESH_VERT_ELEM( mNormals )
		};
		template<> struct TypeInfo<Color_float_RGB>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Color";
			EXTRA_AI_MESH_VERT_ELEM(mColors[0])
		};
		template<> struct TypeInfo<TexCoordUV>
		{
			using type = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
			EXTRA_AI_MESH_VERT_ELEM(mTextureCoords[0])
		};
		template<> struct TypeInfo<Tangent>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "TANGENT";
			EXTRA_AI_MESH_VERT_ELEM(mTangents)
		};
		template<> struct TypeInfo<Bitangent>
		{
			using type = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "BITANGENT";
			EXTRA_AI_MESH_VERT_ELEM(mBitangents)
		};

		// Each vertex element contains its type and its offset, in bytes, into the vertexlayout
		// which can be thought of as the concrete vertex type.
		class Element
		{
		public:
			Element( ElementType type, size_t offset );
			size_t GetOffset() const;
			size_t GetNextSlot() const;
			size_t Size() const;
			ElementType GetType() const;
			D3D11_INPUT_ELEMENT_DESC GetD3DDesc() const;
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
		VertexLayout& Append( ElementType el ) noexcept;
		bool Try_append( ElementType el ) noexcept;
		bool Contains( ElementType type ) const;
		size_t NumElements() const;
		size_t SizeBytes() const;
		const Element& ResolveByIndex( size_t index ) const;
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
		static constexpr size_t SizeOfElement( ElementType el );
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DInputLayout() const;
		std::string GetUID() const;
	private:
		std::vector<Element> elements;
	};

	// Vertex view class to factor out common data to each vertex and to manage data on a 
	// per vertex basis
	class VertexView
	{
		friend class VertexBuffer;
	public:
		VertexView( char* pData, VertexLayout& layout );
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

				#define X(el)\
				case types::el:\
					SetAttribute<types::el>(pAttr,std::forward<T>(attr));\
					break;
				VERTEX_SUPPORTED_TYPES
				#undef X
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
		CVertexView( const VertexView& vertex );
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
		VertexBuffer( VertexLayout layout );
		// Populate buffer from aimesh
		VertexBuffer( VertexLayout layout, const aiMesh& mesh );
		VertexBuffer() = default;
		void SetLayout( VertexLayout layout );

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
		void Reserve( size_t nVertices );
		VertexView Back();
		VertexView Front();
		VertexView operator[]( size_t index )
		{
			assert( data.size() >= layout.SizeBytes() );
			assert( index < Size() );
			return VertexView( data.data() + ( layout.SizeBytes() * index ), layout );
		}
		CVertexView Back() const;
		CVertexView Front() const;
		CVertexView operator[]( size_t index ) const
		{
			return CVertexView(const_cast<VertexBuffer&>( *this )[index]);
		}
		const char* GetData() const;
		size_t SizeBytes() const;
		size_t Size() const;
		size_t VertexSizeBytes() const;
		const VertexLayout& GetLayout() const;
	private:
		template<VertexLayout::ElementType Type>
		void SetVertsByAiMesh( const aiMesh& mesh )
		{
			// called once for each vertex element type from aimesh ctor

			// for each vert in aimesh, set component
			for ( size_t i = 0; i < mesh.mNumVertices; i++ )
			{
				( *this )[i].Attribute<Type>() = VertexLayout::TypeInfo<Type>::Extract( mesh, i );
			}
		}
	private:
		VertexLayout layout;
		std::vector<char> data;
	};
};