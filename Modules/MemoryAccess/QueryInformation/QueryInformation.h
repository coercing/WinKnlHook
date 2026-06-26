#pragma once
#include "../../../Includes.h"

typedef struct _processinfo {
	ULONG pid;
	PROCESS_BASIC_INFORMATION processInfo;
} processinfo, * pprocessinfo;


namespace QueryProcessInfo
{
	auto QueryProcessInfo(pprocessinfo request) -> NTSTATUS;
}
