#pragma once
#include"Win.h"
#include "Loguru/loguru.hpp"
#include "GeneralUtilities.h"
#include <vector>
#include <sstream>

class LoguruManager
{
public:
	//pls be temp class to test output logging to debug console
	class Test
	{

	};
public:
	LoguruManager()
	{
		// Fake the cmd arguments cause I can't be bothered to parse them from winapi
		std::vector<const char*> strArgs = { "fakeargslol", "-v", "OFF" };
		int argc = (int)strArgs.size();
		std::vector<char*> argv;
		for ( const auto& a : strArgs )
			argv.push_back( const_cast<char*>(a) );

		loguru::init( argc, argv.data() );

		// Setup files
		loguru::add_file( "logs\\everything.log", loguru::Truncate, loguru::Verbosity_MAX );
		loguru::add_file( "logs\\attention.log", loguru::Truncate, loguru::Verbosity_WARNING );

		loguru::add_callback( "vs_debug_output", Log_Debug, nullptr, loguru::Verbosity_WARNING);
	}
private:
	static void Log_Debug( void* user_data, const loguru::Message& message )
	{
		std::ostringstream oss;
		oss << message.prefix << "  " << message.message << '\n';
		OutputDebugStringA( oss.str().c_str() );
	}
};