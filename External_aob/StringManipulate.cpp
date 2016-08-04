#include "StringManipulate.hpp"
#include <locale>
#include <codecvt>

bool StringManipulate::EndsWith(std::wstring const &str, std::wstring const &end)
{
	auto len = static_cast<signed>(end.length());
	auto pos = static_cast<signed>(str.length()) - len;
	if(pos < 0)
		return false;
	auto pos_a = &str[pos];
	auto pos_b = &end[0];
	while(*pos_a)
		if(*pos_a++ != *pos_b++)
			return false;
	return true;
}

bool StringManipulate::EndsWith(std::string const &str, std::string const &end)
{
	auto len = static_cast<signed>(end.length());
	auto pos = static_cast<signed>(str.length()) - len;
	if(pos < 0)
		return false;
	auto pos_a = &str[pos];
	auto pos_b = &end[0];
	while(*pos_a)
		if(*pos_a++ != *pos_b++)
			return false;
	return true;
}

std::vector<std::string> StringManipulate::SplitString(const std::string &str, const std::string &delimiter)
{
	std::vector<std::string> Return;
	auto start = 0U;
	auto end = str.find(delimiter);
	while(end != std::string::npos)
	{
		Return.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}

	Return.push_back(str.substr(start, end));
	return Return;
}

std::string StringManipulate::WstringToString(const std::wstring &str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(str);
}

std::wstring StringManipulate::StringToWString(const std::string &str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string StringManipulate::Trim(const std::string &str)
{
	size_t first = str.find_first_not_of(' ');
	if(first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
