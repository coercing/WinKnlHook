#pragma once

#include "../../../Includes.h"



typedef struct _dtb {
	uint64_t BaseAddress;
	uint64_t outaddress;
} dtb, * pdtb;



namespace PML4
{
	inline ULONGLONG suDB = 0;
	inline INT32 TargetPID = 0;
	PVOID fpim(const void* startAddress, size_t memorySize, const void* pattern, size_t patternSize);
	NTSTATUS initDb();
	static auto DirbaseFromBaseAddress(void* processBase) -> uint64_t;
	NTSTATUS DecryptCR3(pdtb request);
}
