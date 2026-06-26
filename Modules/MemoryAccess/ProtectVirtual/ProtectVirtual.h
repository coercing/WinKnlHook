#pragma once
#include "../../../Includes.h"

typedef struct _protect {
	ULONG pid;
	PVOID addr;
	SIZE_T size;
	ULONG protect;
} protect, * pprotect;

namespace ProtectVirtualMem
{
	auto ProtectVirtualMemory(pprotect request) -> NTSTATUS;
}