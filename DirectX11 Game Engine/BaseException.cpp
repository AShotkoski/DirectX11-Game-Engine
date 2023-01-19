#include "BaseException.h"
#include <sstream>

BaseException::BaseException( int line, const std::string& file ) noexcept
	:
	line(line),
	file(file)
{
}

const char* BaseException::what() const noexcept
{
	std::ostringstream ss;
	ss << GetType() << std::endl <<
		"In file: " << GetFile() << std::endl <<
		"On line: " << GetLine() << std::endl;
	// Whatbuffer is used because the std::Exception what func returns char*
	whatBuffer = ss.str();
	return whatBuffer.c_str();
}

const char* BaseException::GetType() const noexcept
{
	return "Undefined Handled Exception";
}

int BaseException::GetLine() const noexcept
{
	return line;
}

const std::string& BaseException::GetFile() const noexcept
{
	return file;
}
