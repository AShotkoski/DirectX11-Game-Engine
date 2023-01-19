#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception( int line, const std::string& file ) noexcept;
	const char* what() const noexcept;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
private:
	int line;
	std::string file;
private:
	// Used for what function since pointer
	mutable std::string whatBuffer;
};

