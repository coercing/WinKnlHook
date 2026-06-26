#include "FreeVirtual.h"


namespace FreeVirtualMem
{
	auto FreeVirtualMemory(pfree request) -> NTSTATUS
	{
        if (request == NULL || request->pid == 0) {
            printf("[-] FreeVirtualMemory hit STATUS_INVALID_PARAMETER.\n");
            return STATUS_INVALID_PARAMETER;
        }
        PEPROCESS Process = NULL;
        if (!NT_SUCCESS((NT.PsLookupProcessByProcessId)((HANDLE)request->pid, &Process))) {
            printf("[-] Couldn't Find ProcessID.\n");
            return STATUS_UNSUCCESSFUL;
        }
        printf("[+] Found Process -> %d.\n", request->pid);
        KAPC_STATE apc;
        NT.KeStackAttachProcess(Process, &apc);

        NTSTATUS FreeVirtual = NT.ZwFreeVirtualMemory(ZwCurrentProcess(), &request->addr, &request->size, request->type);
        if (!NT_SUCCESS(FreeVirtual))
        {
            printf("[-] ZwFreeVirtualMemory failed with status: 0x%X.\n", FreeVirtual);
            NT.KeUnstackDetachProcess(&apc);
            NT.ObfDereferenceObject(Process);
            return FreeVirtual;
        }

        NT.KeUnstackDetachProcess(&apc);

        printf("[+] ZwFreeVirtualMemory succeeded.\n");
        (NT.ObfDereferenceObject)(Process);
        return STATUS_SUCCESS;

	}
}