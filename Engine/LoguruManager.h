#pragma once
#include "Loguru/loguru.hpp"

class LoguruManager
{
public:
	LoguruManager();
private:
	static void Log_Debug( void* user_data, const loguru::Message& message );
};