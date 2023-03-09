#pragma once
#include <Graphics/Graphics.h>
#include "GraphResource.h"
#include <string>
#include <functional>

namespace RDG
{
	//class Pass
	//{
	//public:
	//	Pass( const std::string& name )
	//		: name_( name )
	//	{}
	//	virtual void Execute();
	//	virtual ~Pass() = default;
	//private:
	//	std::string name_;
	//};

	template <class Recs>
	class Pass
	{
	public:
		Pass( const std::string& name, Recs SinksSources, const std::function<void ()>& execute)
			: name_( name )
			, fExec(std::move( execute ))
		{			
			for ( auto& s : SinksSources.Sinks )
			{
				Sinks.push_back( std::move( s ) );
			}	
			for ( auto& s : SinksSources.Sources )
			{
				Sources.push_back( std::move( s ) );
			}


		}
		void Execute()
		{
			fExec();
		}
	private:
		std::string name_;
		std::vector<std::shared_ptr<GraphResource>> Sinks;
		std::vector<std::shared_ptr<GraphResource>> Sources;
		std::function<void ()> fExec;
	};
}