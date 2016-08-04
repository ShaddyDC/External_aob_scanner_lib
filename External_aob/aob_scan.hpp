#pragma once
#include <Windows.h>
#include <vector>

class aob_scan
{
public:
	static int scan(HANDLE handle, const std::string &pattern, const int memory_flags = PAGE_EXECUTE_READWRITE);
	static constexpr int no_result = -1;
private:
	struct Aob_byte
	{
		BYTE byte;
		bool enabled;
	};

	static std::vector<Aob_byte> convert_pattern(const std::string &pattern);
	static bool check_pattern(const std::vector<BYTE> memory_bytes, const std::vector<Aob_byte> &signature);
	static int scan_region(const HANDLE handle, const int address, const int end_address,
	                       const std::vector<Aob_byte> &signature);
	static bool scan_conditions(const int memory_flags, const MEMORY_BASIC_INFORMATION &memory_information);
};
