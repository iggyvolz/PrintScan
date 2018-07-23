#pragma once
#include<vector>
#include <sane/sane.h>
#include "ScannedImage.h"
#include<string>
class Scanner
{
    public:
        virtual ~Scanner();
        SANE_Status Open();
        const std::string GetName();
		const std::string GetIdentifier();
        friend class ScannerEnvironment;
		friend class ScannedImage;
		std::vector<const SANE_Option_Descriptor*> Options;
		void GetOptions();

		template<typename T>
		// Gets the current value of an option
		T GetCurrentValue(std::size_t index, std::size_t size=sizeof(T), int* info=nullptr, SANE_Status* status = nullptr);

		template<typename T>
		// Gets the current value of an option
		T* GetCurrentValuePointer(std::size_t index, std::size_t size=sizeof(T), int* info=nullptr, SANE_Status* status = nullptr);

		template<typename T>
		// Sets the current value of an option
		SANE_Status SetCurrentValue(std::size_t index, T value, std::size_t size = sizeof(T), int* info = nullptr);

		template<typename T>
		// Sets the current value of an option
		SANE_Status SetCurrentValuePointer(std::size_t index, T* value, std::size_t size = sizeof(T), int* info = nullptr);

		ScannedImage* StartScan();
    private:
        Scanner(SANE_Device);
        SANE_Handle handle;
        SANE_Device device;
		void* _GetCurrentValue(std::size_t index, std::size_t size, int* info, SANE_Status* status);
		SANE_Status _SetCurrentValue(std::size_t index, void* value, std::size_t size, int* info);
		ScannedImage* scannedImage=nullptr;
};
template<typename T>
T Scanner::GetCurrentValue(std::size_t index, std::size_t size, int* info, SANE_Status* status)
{
	T* ptr = GetCurrentValuePointer<T>(index, size, info, status);
	T value = *ptr;
	free(ptr);
	return value;
}

template<typename T>
T* Scanner::GetCurrentValuePointer(std::size_t index, std::size_t size, int* info, SANE_Status* status)
{
	void* ptr = Scanner::_GetCurrentValue(index, size, info, status);
	// Dereference the pointer and return its value as the designated type
	return (T*)ptr;
}
template<typename T>
SANE_Status Scanner::SetCurrentValue(std::size_t index, T value, std::size_t size, int* info)
{
	T* ptr = (T*)malloc(size);
	(*ptr) = value;
	SANE_Status s = Scanner::SetCurrentValuePointer(index, ptr, size, info);
	free(ptr);
	return s;
}

template<typename T>
SANE_Status Scanner::SetCurrentValuePointer(std::size_t index, T* value, std::size_t size, int* info)
{
	return Scanner::_SetCurrentValue(index, (void*)value, size, info);
}