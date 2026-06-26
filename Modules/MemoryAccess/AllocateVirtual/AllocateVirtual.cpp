#include "AllocateVirtual.h"

namespace AllocateVirtualMem
{
	auto AllocateVirtualMemory(pallocate request) -> NTSTATUS
	{
        if (request == NULL || request->pid == 0) {
            printf("[-] AllocateVirtualMemory hit STATUS_INVALID_PARAMETER.\n");
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

        NTSTATUS AllocateVirtual = NT.ZwAllocateVirtualMemory(ZwCurrentProcess(), &request->address, 0, &request->size, request->type, request->protect);
        if (!NT_SUCCESS(AllocateVirtual))
        {
            printf("[-] ZwAllocateVirtualMemory failed with status: 0x%X.\n", AllocateVirtual);
            NT.KeUnstackDetachProcess(&apc);
            NT.ObfDereferenceObject(Process);
            return AllocateVirtual;
        }

        // CRITICAL: Touch ALL pages in the allocated region to commit them
        // This forces the page tables to be populated for all pages
        __try {
            // Touch one byte in each 4KB page to commit all pages
            const size_t page_size = 0x1000; // 4KB
            size_t pages_to_touch = (request->size + page_size - 1) / page_size; // Round up
            
            printf("[+] Touching %llu pages to commit physical memory...\n", pages_to_touch);
            for (size_t i = 0; i < pages_to_touch; i++) {
                BYTE* page_addr = reinterpret_cast<BYTE*>(request->address) + (i * page_size);
                if (page_addr < reinterpret_cast<BYTE*>(request->address) + request->size) {
                    volatile BYTE dummy = *page_addr;  // Read first byte of each page
                    *page_addr = dummy;  // Write it back (triggers page fault if needed)
                }
            }
            printf("[+] Successfully committed %llu pages\n", pages_to_touch);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            printf("[-] Exception while touching memory: 0x%X\n", GetExceptionCode());
        }

        NT.KeUnstackDetachProcess(&apc);

        printf("[+] Found ZwAllocateVirtualMemory -> 0x%llx.\n", AllocateVirtual);
        (NT.ObfDereferenceObject)(Process);
        return STATUS_SUCCESS;

	}
}