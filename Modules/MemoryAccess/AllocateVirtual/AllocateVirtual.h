#pragma once
#include "../../../Includes.h"

typedef struct _allocate {
	ULONG pid;
	PVOID address;
	SIZE_T size;
	ULONG type;
	ULONG protect;
} allocate, * pallocate;

namespace AllocateVirtualMem
{
	auto AllocateVirtualMemory(pallocate request) -> NTSTATUS;
}