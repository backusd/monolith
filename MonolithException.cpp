#include "MonolithException.h"
#include <sstream>


MonolithException::MonolithException(int line, const char* file) noexcept :
	line(line),
	file(file)
{
}

const char* MonolithException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* MonolithException::GetType() const noexcept
{
	return "Monolith Exception";
}

int MonolithException::GetLine() const noexcept
{
	return line;
}

const std::string& MonolithException::GetFile() const noexcept
{
	return file;
}

std::string MonolithException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
