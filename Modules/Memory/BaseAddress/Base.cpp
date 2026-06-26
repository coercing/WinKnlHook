#include "Base.h"


namespace BaseAddy
{

    //NTSTATUS GetBaseAddress(pbase request)
    //{
    //    if (!request || !request->MProcess)
    //        return STATUS_INVALID_PARAMETER;

    //    request->Address =
    //        reinterpret_cast<uintptr_t*>(
    //            NT.PsGetProcessSectionBaseAddress(request->MProcess)
    //            );

    //    return request->Address ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
    //}

    //old way u can use
    NTSTATUS BaseAddress(pbase_new request)
    {
        if (request->process_id == 0) {
            printf("[-] BaseAddress hit STATUS_INVALID_PARAMETER.\n");
            return STATUS_INVALID_PARAMETER;
        }
        PEPROCESS Process = NULL;
        if (!NT_SUCCESS((NT.PsLookupProcessByProcessId)((HANDLE)request->process_id, &Process))) {
            printf("[-] Couldn't Find ProcessID.\n");
            return STATUS_UNSUCCESSFUL;
        }
        printf("[+] Found Process -> %d.\n", request->process_id);

        ULONGLONG ProcessBase = (ULONGLONG)(NT.PsGetProcessSectionBaseAddress)(Process);
        if (!ProcessBase)
        {
            printf("[-] Couldn't Get BaseAddress.\n");
            return STATUS_INVALID_PARAMETER;
        }
        printf("[+] Found BaseAddress -> 0x%llx.\n", ProcessBase);
        request->BaseAddress = ProcessBase;
        
        PML4::TargetPID = request->process_id;
        
        (NT.ObfDereferenceObject)(Process);
        return STATUS_SUCCESS;
    }

  /*NTSTATUS GetBaseAddress(pbase request)
    {
        if (!request || !request->MProcess)
            return STATUS_INVALID_PARAMETER;
        request->Address =
            reinterpret_cast<uintptr_t*>(
                NT.PsGetProcessSectionBaseAddress(request->MProcess)
                );
        return request->Address ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
    }*/
}