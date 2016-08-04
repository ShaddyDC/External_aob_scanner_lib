#include "aob_scan.hpp"
#include "StringManipulate.hpp"

int aob_scan::scan(HANDLE handle, const std::string &pattern, const int memory_flags)
{
	auto signature = convert_pattern(pattern);

	//Get address range
	SYSTEM_INFO system_information;
	GetSystemInfo(&system_information);
	const int start_address = reinterpret_cast<int>(system_information.lpMinimumApplicationAddress);
	const int end_address = reinterpret_cast<int>(system_information.lpMaximumApplicationAddress);


	//Iterate through memory regions
	MEMORY_BASIC_INFORMATION memory_information = { };
	for(int i = start_address; i < end_address - static_cast<int>(signature.size()); i += memory_information.RegionSize)	//vectors have random access iterators so < works
	{
		//Get region data
		if(VirtualQueryEx(handle, reinterpret_cast<LPVOID>(i), &memory_information, sizeof memory_information) == 0)
			throw std::runtime_error{ "Couldn't retrieve memory information! Winerror: " + std::to_string(GetLastError()) };

		if(scan_conditions(memory_flags, memory_information))
		{
			//Scan region
			auto ret = scan_region(handle, i, i + memory_information.RegionSize, signature); // Returns the offset of the address within the memory region
			if(ret != no_result)
				return i + ret; 
		}
	}
	return no_result;
}


bool aob_scan::check_pattern(const std::vector<BYTE> memory_bytes, const std::vector<Aob_byte> &signature)
{
	// Checks if both vectors match
	for(int i = 0; i < static_cast<int>(signature.size()); i++)
		if(signature.at(i).enabled && signature.at(i).byte != memory_bytes.at(i)) 
			return false;
	return true;
}


int aob_scan::scan_region(const HANDLE handle, const int address, const int endAddress, const std::vector<Aob_byte> &signature)
{
	//Reads memory or region
	const int size = endAddress - address;
	std::vector<BYTE> memory_bytes(size); 
	ReadProcessMemory(handle, reinterpret_cast<LPVOID>(address), memory_bytes.data(), size, nullptr); 

	//Loops through region
	for(auto i = memory_bytes.cbegin(); i != memory_bytes.end() - signature.size(); ++i)
	{
		if(check_pattern({i, i + signature.size()}, signature))
			return i - memory_bytes.cbegin();
	}
	return no_result;
}

bool aob_scan::scan_conditions(const int memory_flags, const MEMORY_BASIC_INFORMATION &memory_information)
{
	//Checks if region should be scanned
	return (memory_information.State & MEM_COMMIT) //Actually used memory
		&& (memory_information.Protect & memory_flags)	//Matches the desired memory "mode"
		&& !(memory_information.Protect & PAGE_GUARD);	//We don't ever want this, I think
}

std::vector<aob_scan::Aob_byte> aob_scan::convert_pattern(const std::string &pattern)
{
	//Converts cheatengine like pattern string to data that can easily be used

	std::vector<Aob_byte> ret;

	auto parts = StringManipulate::SplitString(pattern, " ");

	for(const auto &part : parts)
	{
		if(part == "??")
			ret.emplace_back(Aob_byte{ 0, false });
		else
			ret.emplace_back(Aob_byte{ static_cast<BYTE>(std::stoi(part, nullptr, 16)), true });
	}
	return ret;
}
