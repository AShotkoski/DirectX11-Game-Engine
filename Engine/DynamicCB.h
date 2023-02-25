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
			// used for setting offset
			friend class Layout;
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
		Layout()
			: isAligned_( false )
		{}
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
			// todo fix

		}
		bool isAligned() const
		{
			return isAligned_;
		}
		// ONLY CALLED FROM Buffer ( todo some funky inheritance to do this )
		void Align();
		size_t GetSizeBytes() const
		{
			DCHECK_F( isAligned(), "Attempted to get size of non aligned layout." );
			return elements_.back().offset() + GetTypeSysSize( elements_.back().type() );
		}
	private:
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
	private:
		std::vector<Element> elements_;
		bool isAligned_;
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
		Buffer( Layout layout );
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