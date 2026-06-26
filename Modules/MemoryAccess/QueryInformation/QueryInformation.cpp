#include "QueryInformation.h"


namespace QueryProcessInfo
{
	auto QueryProcessInfo(pprocessinfo request) -> NTSTATUS
	{
        if (request == NULL || request->pid == 0) {
            printf("[-] QueryProcessInfo hit STATUS_INVALID_PARAMETER.\n");
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

        NTSTATUS QueryInfo = NT.ZwQueryInformationProcess(ZwCurrentProcess(), ProcessBasicInformation, &request->processInfo, sizeof(request->processInfo), NULL);
        if (!NT_SUCCESS(QueryInfo))
        {
            printf("[-] ZwQueryInformationProcess failed with status: 0x%X.\n", QueryInfo);
            NT.KeUnstackDetachProcess(&apc);
            NT.ObfDereferenceObject(Process);
            return QueryInfo;
        }

        NT.KeUnstackDetachProcess(&apc);

        printf("[+] ZwQueryInformationProcess succeeded.\n");
        (NT.ObfDereferenceObject)(Process);
        return STATUS_SUCCESS;

	}
}
