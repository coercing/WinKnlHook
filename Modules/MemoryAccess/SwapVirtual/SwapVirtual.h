#pragma once


#include "../../../Includes.h"
typedef struct _swap {
	ULONG TargetPID;
	uintptr_t Address;
	uintptr_t Address2;
	uintptr_t Address3;
} swap, * pswap;

namespace SwapVirtual
{
	NTSTATUS SwapVirtual(pswap Buffer);
}