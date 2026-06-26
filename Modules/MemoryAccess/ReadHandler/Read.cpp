#include "Read.h"

namespace Read
{
    auto ReadHandler(pread request) -> NTSTATUS
    {
        if (!request || !request->address || !request->buffer || !request->size)
            return STATUS_INVALID_PARAMETER;

        SIZE_T total_bytes_read = 0;
        UINT64 current_address = request->address;
        UINT64 current_buffer = request->buffer;
        SIZE_T remaining_size = request->size;

        while (remaining_size > 0) {
            UINT64 phys_addr = RWX::TranslateLinear(PML4::suDB, current_address);
            if (!phys_addr) 
                return STATUS_UNSUCCESSFUL;

            ULONG64 final_size = (PAGE_SIZE - (phys_addr & 0xFFF)) < remaining_size ?
                (PAGE_SIZE - (phys_addr & 0xFFF)) : remaining_size;

            SIZE_T bytes = 0;
            PVOID temp_buffer = ExAllocatePoolWithTag(NonPagedPool, final_size, 'kair');
            if (!temp_buffer) 
                return STATUS_UNSUCCESSFUL;

            NTSTATUS status = RWX::ReadMemory(phys_addr, temp_buffer, final_size, &bytes);
            
            if (NT_SUCCESS(status)) {
                __try {
                    ProbeForWrite((PVOID)current_buffer, bytes, 1);
                    RtlCopyMemory((PVOID)current_buffer, temp_buffer, bytes);
                    total_bytes_read += bytes;
                    current_address += bytes;
                    current_buffer += bytes;
                    remaining_size -= bytes;
                }
                __except (EXCEPTION_EXECUTE_HANDLER) {
                    ExFreePoolWithTag(temp_buffer, 'kair');
                    return STATUS_UNSUCCESSFUL;
                }
            } else {
                ExFreePoolWithTag(temp_buffer, 'kair');
                return status;
            }

            ExFreePoolWithTag(temp_buffer, 'kair');
        }

        return STATUS_SUCCESS;
    }
}