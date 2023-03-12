#include "Win.h"
#include "LoguruManager.h"
#include "Util/GeneralUtilities.h"
#include "Core/BaseException.h"
#include <vector>
#include <sstream>

class LogException : public BaseException
{
public:
	LogException( const loguru::Message& message )
		: BaseException(message.line, message.filename)
		, message(std::string(message.prefix) + message.message)
	{}
	virtual const char* what() const noexcept override
	{
		std::ostringstream ss;
		ss <<
			"[File] " << GetFile() << std::endl <<
			"[Line] " << GetLine() << std::endl <<
			"[Message] " << message << std::endl;
		
		whatBuffer = ss.str();
		return whatBuffer.c_str();
	}
	virtual const char* GetType() const noexcept override
	{
		return "Log Check Exception";
	}
private:
	const std::string message;
};

LoguruManager::LoguruManager()
{
	// Fake the cmd arguments cause I can't be bothered to parse them from winapi
	std::vector<const char*> strArgs = { "fakeargslol", "-v", "OFF" };
	int argc = (int)strArgs.size();
	std::vector<char*> argv;
	for ( const auto& a : strArgs )
		argv.push_back( const_cast<char*>( a ) );

	loguru::init( argc, argv.data() );

	// Setup files
	loguru::add_file( "logs\\everything.log", loguru::Truncate, loguru::Verbosity_MAX );
	loguru::add_file( "logs\\attention.log", loguru::Truncate, loguru::Verbosity_WARNING );
#ifdef _DEBUG
	loguru::add_callback( "vs_debug_output", Log_Debug, nullptr, loguru::Verbosity_WARNING );
#endif
	// todo add our own exception class here
	loguru::set_fatal_handler( []( const loguru::Message& message ) 							   
							   {
									throw LogException(message);
							   } );
}

void LoguruManager::Log_Debug( void* user_data, const loguru::Message& message )
{
	std::ostringstream oss;
	oss << message.prefix << "  " << message.message << '\n';
	OutputDebugStringA( oss.str().c_str() );
}
