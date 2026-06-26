#include "Peb.h"

extern "C" NTKERNELAPI PPEB NTAPI PsGetProcessPeb(IN PEPROCESS Process);

namespace Peb
{
	NTSTATUS GetPEB(ppeb Buffer)
	{
		if (!Buffer) return STATUS_INVALID_PARAMETER;

		PEPROCESS Process = nullptr;
		NTSTATUS status = PsLookupProcessByProcessId((HANDLE)(ULONG_PTR)Buffer->TargetPID, &Process);
		if (!NT_SUCCESS(status)) return status;

		Buffer->Address = uintptr_t(PsGetProcessPeb(Process));
		NT.ObfDereferenceObject(Process);
		return STATUS_SUCCESS;
	}
}