#pragma once
#include "../../../Includes.h"

typedef struct _free {
	ULONG pid;
	PVOID addr;
	SIZE_T size;
	ULONG type;
} freee, * pfree;


namespace FreeVirtualMem
{
	auto FreeVirtualMemory(pfree request) -> NTSTATUS;
}