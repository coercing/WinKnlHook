#pragma once

#include "../../../Includes.h"

typedef struct _wrtie {
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} write, * pwrite;

namespace Write
{
	auto WriteHandler(pwrite request) -> NTSTATUS;
}