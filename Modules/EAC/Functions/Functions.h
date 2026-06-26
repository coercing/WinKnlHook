#pragma once
#include "../../../Includes.h"





namespace EACFunctions
{

	NTSTATUS ReadMemory(uint64_t destination, PVOID src, SIZE_T size, SIZE_T* copiedSize);
	PVOID GetProcessBaseAddress(PEPROCESS process);
	uintptr_t GetSystemProcess();
	BOOLEAN IsAddressValid(PVOID address);
	uint32_t GetProcessId(PEPROCESS* process);
}