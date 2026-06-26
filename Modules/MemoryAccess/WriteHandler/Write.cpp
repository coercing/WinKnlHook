#include "Write.h"

namespace Write
{
	auto WriteHandler(pwrite request) -> NTSTATUS
	{
		if (!request || !request->address || !request->buffer || !request->size)
			return STATUS_INVALID_PARAMETER;

		SIZE_T total_bytes_written = 0;
		UINT64 current_address = request->address;
		UINT64 current_buffer = request->buffer;
		SIZE_T remaining_size = request->size;

		while (remaining_size > 0) {
			UINT64 phys_addr = RWX::TranslateLinear(PML4::suDB, current_address);
			if (!phys_addr) {
				return STATUS_UNSUCCESSFUL;
			}

			ULONG64 final_size = (PAGE_SIZE - (phys_addr & 0xFFF)) < remaining_size ?
				(PAGE_SIZE - (phys_addr & 0xFFF)) : remaining_size;

			SIZE_T bytes = 0;
			PVOID temp_buffer = ExAllocatePoolWithTag(NonPagedPool, final_size, 'kair');
			if (!temp_buffer) {
				return STATUS_UNSUCCESSFUL;
			}

			__try {
				ProbeForRead((PVOID)current_buffer, final_size, 1);
				RtlCopyMemory(temp_buffer, (PVOID)current_buffer, final_size);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				ExFreePoolWithTag(temp_buffer, 'kair');
				return STATUS_UNSUCCESSFUL;
			}
			
			NTSTATUS status = RWX::WritePhysicalAddress(phys_addr, temp_buffer, final_size, &bytes);

			ExFreePoolWithTag(temp_buffer, 'kair');

			if (!NT_SUCCESS(status)) {
				return status;
			}

			total_bytes_written += bytes;
			current_address += bytes;
			current_buffer += bytes;
			remaining_size -= bytes;
		}

		return STATUS_SUCCESS;
	}
}