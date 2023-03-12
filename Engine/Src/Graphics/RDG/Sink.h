#pragma once

#include <string>

#include <log.h>
#include <Binds/Bindable.h>
#include "Source.h"
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
		const std::string& GetTargetPassName() const
		{
			return targetPassName;
		}
		const std::string& GetTargetSourceName() const
		{
			return targetSourceName;
		}

		virtual void Bind(Source& source) = 0;
	protected:
		Sink(const std::string& name)
			: name(name)
		{
			// todo: name validation
		}
	protected:
		bool linked = false;
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
		virtual void BufferSink::Bind( Source& source ) override
		{
			auto rec = std::dynamic_pointer_cast<T>( source.YieldResource() );
			DCHECK_NOTNULL_F( rec.get(), "Failed resource acquisition when binding %s to %s.", source.GetName().c_str(), GetName().c_str());
			
			pResource = std::move( rec );
			linked = true;
		}
	private:
		std::shared_ptr<T>& pResource;
	};
}