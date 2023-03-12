#pragma once

#include <string>

class BufferResource;

namespace RDG
{
	class Source
	{
	public:
		virtual ~Source() = default;
		const std::string& GetName() const
		{
			return name;
		}
		virtual std::shared_ptr<BufferResource> YieldResource();
	protected:
		Source( const std::string& name )
			: name( name )
		{
			// todo: name validation
		}
	private:
		std::string name;
	};

	// The template type is the type of resource the Source handles
	template <typename T>
		requires std::is_base_of<BufferResource, T>::value
	class BufferSource : public Source
	{
	public:
		BufferSource( const std::string& name, std::shared_ptr<T>& pTarget )
			: Source( name )
		{}
		// Helper to make unique ptr
		static std::unique_ptr<BufferSource<T>> MakeUnique( const std::string& name, std::shared_ptr<T>& pTarget )
		{
			return std::make_shared<BufferSource<T>>( name, pTarget );
		}
	private:
		std::shared_ptr<T>& pResource;
	};
}