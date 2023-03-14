#pragma once

#include <string>

class BufferResource;

class Bindable;

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
		virtual std::shared_ptr<Bindable> YieldResource() = 0;
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
		requires std::is_base_of<Bindable, T>::value
	class BufferSource : public Source
	{
	public:
		BufferSource( const std::string& name, std::shared_ptr<T>& pTarget )
			: Source( name )
			, pResource(pTarget)
		{}
		// Helper to make unique ptr
		static std::unique_ptr<BufferSource<T>> MakeUnique( const std::string& name, std::shared_ptr<T>& pTarget )
		{
			return std::make_unique<BufferSource<T>>( name, pTarget );
		}
		virtual std::shared_ptr<Bindable> YieldResource() override
		{
			return pResource;
		}
	private:
		std::shared_ptr<T>& pResource;
	};
}