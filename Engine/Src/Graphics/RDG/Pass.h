#pragma once
#include <Graphics/Graphics.h>
#include <string>
#include <vector>


namespace RDG
{
	class Sink;
	class Source;

	class Pass
	{
	public:
		Pass( const std::string& name );
		virtual void Execute(Graphics& gfx) = 0;
		virtual ~Pass() = default;
		// Target is target pass and target source separated by a period eg "lambertian.gbuffer"
		void SetSinkLinkage( const std::string& sink_name, const std::string& target );
		Sink& GetSink( const std::string& sink_name );
		const std::vector<std::unique_ptr<Sink>>& GetSinks() const;
		Source& GetSource( const std::string& source_name );
		const std::string& GetName() const;
		// Check that all sinks are linked
		bool ValidateLinkage() const;
	protected:
		void RegisterSink( std::unique_ptr<Sink> pSink );
		void RegisterSource( std::unique_ptr<Source> pSource );
	private:
		std::string name_;
		std::vector<std::unique_ptr<Sink>> SinkPtrs;
		std::vector<std::unique_ptr<Source>> SourcePtrs;
	};

}