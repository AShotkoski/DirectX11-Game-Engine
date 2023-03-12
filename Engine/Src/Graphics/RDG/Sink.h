#pragma once

#include <string>

//
// A sink is bound to a pass and is used to describe the linkage of the graph.
// A sink will always be bound/attached to a pass, and must always be linked to a source.
//

class BufferResource;

namespace RDG
{
	
	class Sink
	{
	public:
		virtual ~Sink() = default;
		void SetTarget( const std::string& pass_name, const std::string& source_name )
		{
			// Assumes names are previously validated
			targetPassName = pass_name;
			targetSourceName = source_name;
		}
		const std::string& GetName() const
		{
			return name;
		}
	protected:
		Sink(const std::string& name)
			: name(name)
		{
			// todo: name validation
		}
	private:		
		std::string name;
		std::string targetPassName;
		std::string targetSourceName;
	};

	// The template type is the type of resource the sink handles
	template <typename T>
		requires std::is_base_of<Bindable, T>::value
	class BufferSink : public Sink
	{
	public:
		BufferSink( const std::string& name, std::shared_ptr<T>& pTarget)
			: Sink(name)
			, pResource(pTarget)
		{}
		// Helper to make unique ptr
		static std::unique_ptr<BufferSink<T>> MakeUnique( const std::string& name, std::shared_ptr<T>& pTarget )
		{
			return std::make_unique<BufferSink<T>>( name, pTarget );
		}
	private:
		std::shared_ptr<T>& pResource;
	};
}