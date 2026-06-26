#pragma once

#include "../../../Includes.h"

typedef struct _read {
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} read, * pread;

namespace Read
{
	auto ReadHandler(pread request) -> NTSTATUS;
}