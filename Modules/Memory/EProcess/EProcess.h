#pragma once
#include "../../../Includes.h"

typedef struct _processid {
	INT32 ProcessID;
	PEPROCESS* MProcess;
} processid, * pprocessid;
namespace EProcess
{
	NTSTATUS GetEProcess(pprocessid request);
}