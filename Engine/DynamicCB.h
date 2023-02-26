#pragma once
#include "Loguru/loguru.hpp"
#include <DirectXMath.h>
#include <string>
#include <type_traits>
#include <vector>
#include <optional>

namespace CB
{
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
		Invalid
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
	template<>
	struct TypeInfo<Invalid>
	{
		using systype = void;
		static constexpr const size_t hlslsize = 0u;
	};

	// Heart of the Dynamic CB system. At its core just a vector of Element objects (element
	// is a basic class which just contains a data type and name, and once aligned, an offset) 
	// which describe the layout of the constant buffer.

	class Layout
	{
		friend class View;
	private:
		// Class which holds the necessary information of each element in the buffer
		class Element
		{
			// used for setting offset
			friend class Layout;
		public:
			Element( Type type, const std::string& name );
			Type GetType() const;
			const std::string& GetName() const;
			const size_t GetOffset() const;
		private:
			Type type_;
			std::optional<size_t> offset_;
			std::string name_;
		};
	public:
		// Add a named element to the layout, which can later by accessed by name
		Layout& add( Type ty, const std::string& name );
		// Add element if it doesn't exist, otherwise do nothing. Returns true if successful
		bool try_add( Type ty, const std::string& name );
	protected:
		void Align();
		size_t GetSizeBytes() const;
		Type GetTypeAt( const std::string& name ) const;
		size_t GetOffsetAt( const std::string& name ) const;
		bool ContainsElement( const std::string& name ) const;
	private:
		// Get the size (in bytes) of the system type underlying a Type
		static constexpr size_t GetTypeSysSize( Type type );
		const Element& QueryElementByName( const std::string& name ) const;
	private:
		std::vector<Element> elements_;
	};

	// Creating an AlignedLayout from a layout means that all offsets are set in accordance 
	// with hlsl packing rules, and the layout may no longer be modified. This is the type
	// which the Buffer class contains.

	class AlignedLayout : private Layout
	{
		// Gives access to the inherited functions for getting type and offset at name.
		friend class View;
	public:
		AlignedLayout( Layout unaligned );
		size_t GetSizeBytes() const;
		bool Contains( const std::string name ) const;
	};


	/******************** Data View **************************************************************/

	class View
	{
		friend class Buffer;
	public:
		bool Exists() const;
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
		// We only want views to be creatable from Buffer
		View( char* pData, const AlignedLayout& layout, const std::string& name );
		View();
		// Validate that type T is the same type as what this view represents
		template<typename T>
		bool validate( ) const
		{
			switch ( type_ )
			{
				#define X(el) \
				case el:\
						if(typecmp<T, el>())\
						{\
							return true;\
						}\
						else\
						{\
							[[fallthrough]];\
						}
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

	// Todo implement once needed
	class CView
	{
	public:
		
	private:
		const View& view_;
	};

	/******************** Main constant buffer ***************************************************/

	// Note that data is set to '\0' by default.

	class Buffer
	{
	public:
		Buffer( Layout layout );
		View operator[]( const std::string& name );
		size_t sizeBytes() const;
		const char* GetData() const;
		void CopyFrom( const Buffer& other );
	private:
		AlignedLayout layout_;
		std::vector<char> data_;
	};
};