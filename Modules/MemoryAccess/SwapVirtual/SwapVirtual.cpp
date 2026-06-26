#include "SwapVirtual.h"


namespace SwapVirtual
{
	NTSTATUS SwapVirtual(pswap Buffer)
	{
		PEPROCESS Process = nullptr;
		if (!NT_SUCCESS(NT.PsLookupProcessByProcessId((HANDLE)Buffer->TargetPID, &Process))) return STATUS_UNSUCCESSFUL;

		uintptr_t OriginalPointer = 0;
		KAPC_STATE apc;
		NT.KeStackAttachProcess(Process, &apc);
		OriginalPointer = (uintptr_t)_InterlockedExchangePointer((void**)Buffer->Address, (void*)Buffer->Address2);

		NT.KeUnstackDetachProcess(&apc);
		NT.ObfDereferenceObject(Process);
		Buffer->Address3 = OriginalPointer;
		return STATUS_SUCCESS;
	}
}