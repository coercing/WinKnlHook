#pragma once
#include "../../../Includes.h"

typedef struct _peb {
	ULONG TargetPID;
	uintptr_t Address;
} peb, * ppeb;

namespace Peb
{
	NTSTATUS GetPEB(ppeb Buffer);
}