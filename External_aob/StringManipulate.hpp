#pragma once
#include <vector>
#include <string>

namespace StringManipulate
{
	bool EndsWith(std::wstring const &str, std::wstring const &end);
	bool EndsWith(std::string const &str, std::string const &end);

	std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter);

	std::string WstringToString(const std::wstring &str);
	std::wstring StringToWString(const std::string &str);

	std::string Trim(const std::string &str);
}
