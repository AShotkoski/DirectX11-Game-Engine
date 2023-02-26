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
		Layout();
		// Add a named element to the layout, which can later by accessed by name
		Layout& add( Type ty, const std::string& name );
		const Element& QueryElementByName( const std::string& name ) const;
		bool isAligned() const;
		// ONLY CALLED FROM Buffer ( todo some funky inheritance to do this )
		void Align();
		size_t GetSizeBytes() const;
	private:
		// Get the size (in bytes) of the system type underlying a Type
		static constexpr size_t GetTypeSysSize( Type type );
		bool containsElement( const std::string& name ) const;
	private:
		std::vector<Element> elements_;
		bool isAligned_;
	};

	/******************** Data View **************************************************************/
	class View
	{
	public:
		View( char* pData, const Layout::Element& elem );
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
		bool validate( ) const
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
		View operator[]( const std::string& name );
		size_t sizeBytes() const;
	private:
		Layout layout_;
		std::vector<char> data_;
	};
};