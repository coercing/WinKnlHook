#include "ProtectVirtual.h"

namespace ProtectVirtualMem
{
	auto ProtectVirtualMemory(pprotect request) -> NTSTATUS
	{
        if (request == NULL || request->pid == 0) {
            printf("[-] ProtectVirtualMemory hit STATUS_INVALID_PARAMETER.\n");
            return STATUS_INVALID_PARAMETER;
        }
        PEPROCESS Process = NULL;
        if (!NT_SUCCESS((NT.PsLookupProcessByProcessId)((HANDLE)request->pid, &Process))) {
            printf("[-] Couldn't Find ProcessID.\n");
            return STATUS_UNSUCCESSFUL;
        }
        printf("[+] Found Process -> %d.\n", request->pid);
        KAPC_STATE apc;
        ULONG oldProtect = 0;
        NT.KeStackAttachProcess(Process, &apc);

        NTSTATUS ProtectVirtual = NT.ZwProtectVirtualMemory(ZwCurrentProcess(), &request->addr, &request->size, request->protect, &oldProtect);
        if (!NT_SUCCESS(ProtectVirtual))
        {
            printf("[-] ZwProtectVirtualMemory failed with status: 0x%X.\n", ProtectVirtual);
            NT.KeUnstackDetachProcess(&apc);
            NT.ObfDereferenceObject(Process);
            return ProtectVirtual;
        }

        NT.KeUnstackDetachProcess(&apc);

        // Store old protection value back in request
        request->protect = oldProtect;

        printf("[+] ZwProtectVirtualMemory succeeded. Old protection: 0x%X.\n", oldProtect);
        (NT.ObfDereferenceObject)(Process);
        return STATUS_SUCCESS;
	}
}