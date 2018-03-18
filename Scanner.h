#pragma once
#include<vector>
#include <sane/sane.h>
#include<string>
class Scanner
{
    public:
        virtual ~Scanner();
        SANE_Status Open();
        const std::string GetName();
        friend class ScannerEnvironment;
		std::vector<const SANE_Option_Descriptor*> Options;
		void GetOptions();
		template<typename T>
		// Gets the current value of an option
		T GetCurrentValue(std::size_t index);

		template<typename T>
		// Gets the current value of an option, and returns info about it
		T GetCurrentValue(std::size_t index, std::size_t size);


		template<typename T>
		// Gets the current value of an option of a given size
		T GetCurrentValue(std::size_t index, int* info);

		template<typename T>
		// Gets the current value of an option of a given size, and returns info about it
		T GetCurrentValue(std::size_t index, std::size_t size, int* info);
    private:
        Scanner(SANE_Device);
        SANE_Handle handle;
        SANE_Device device;
		void* _GetCurrentValue(std::size_t index, std::size_t size, int* info);
};


template<typename T>
// Gets the current value of an option
T Scanner::GetCurrentValue(std::size_t index)
{
	return Scanner::GetCurrentValue<T>(index, (int*)nullptr);
}
template<typename T>
// Gets the current value of an option, and returns info about it
T Scanner::GetCurrentValue(std::size_t index, std::size_t size)
{
	return Scanner::GetCurrentValue<T>(index, (int*)nullptr, size);
}
template<typename T>
// Gets the current value of an option of a given size
T Scanner::GetCurrentValue(std::size_t index, int* info)
{
	return Scanner::GetCurrentValue<T>(index, sizeof(T), (int*)nullptr);
}
template<typename T>
// Gets the current value of an option of a given size, and returns info about it
T Scanner::GetCurrentValue(std::size_t index, std::size_t size, int* info)
{
	void* ptr = Scanner::GetCurrentValue<void*>(index, size, info);
	// Dereference the pointer and return its value as the designated type
	return *(T*)ptr;
}