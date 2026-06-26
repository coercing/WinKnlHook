#pragma once

#include "../../../Includes.h"


typedef struct _base_new {
	INT32 process_id;
	ULONGLONG BaseAddress;
} base_new, * pbase_new;

typedef struct _base {
    uintptr_t* Address;    // kernel writes base address here
    PEPROCESS MProcess;  // input: pointer to EPROCESS
} base, * pbase;


namespace BaseAddy
{
	NTSTATUS BaseAddress(pbase_new request);
	NTSTATUS GetBaseAddress(pbase request);
}