#pragma once
#include "../../includes.h"
//#define debug

#ifdef debug
#define printf(text, ...) DbgPrintEx(0, 0, E(text), ##__VA_ARGS__)
#else
#define printf(text, ...) ((void)0) // Completely removed in release
#endif


namespace Utility
{
	void CustomMemcpy(const void* dstp, const void* srcp, UINT len);
	bool isBackedByModule(const wchar_t* module_name, void* address);
	int GetWindowsBuildNumber();
	uintptr_t GetNToskrnlBase();
	uintptr_t GetDebugPrintCallbackListOffset();
	LIST_ENTRY* GetDebugPrintCallbackList(uintptr_t ntoskrnl_base);
	uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask);
	uintptr_t FindPattern(uint64_t base, const char* pattern, const char* mask);
	uint64_t GetEacModule(const wchar_t* module_name);
	uint64_t InitRandom();
	ULONG GetRandomPoolTag();
	uintptr_t GetKernelModule(const char* name);
	bool WriteSafe(void* address, void* buffer, const size_t size);
	template <typename t>
	auto GetKernelExport(const uintptr_t base, const char* name) -> t
	{
		return reinterpret_cast<t>(RtlFindExportedRoutineByName(reinterpret_cast<void*>(base), name));
	}
}