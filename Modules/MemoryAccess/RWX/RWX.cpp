#include "RWX.h"
#include "../../../Core/Definitions/Define.h"

#define PAGE_OFFSET_SIZE 12

namespace RWX
{
    cache cached_pml4e[512];

    auto ReadMemory(uint64_t destination, PVOID src, SIZE_T size, SIZE_T* copiedSize) -> NTSTATUS {
        if (!destination || !src || !copiedSize || size == 0)
            return STATUS_UNSUCCESSFUL;

        MM_COPY_ADDRESS srcAddr;
        srcAddr.PhysicalAddress.QuadPart = (LONGLONG)destination;
        
        NTSTATUS status = (NT.MmCopyMemory)(src, srcAddr, size, MM_COPY_MEMORY_PHYSICAL, copiedSize);

        if (!NT_SUCCESS(status))
        {
            *copiedSize = 0;
            RtlZeroMemory(src, size);
        }

        return status;
    }

    auto WritePhysicalAddress(uint64_t target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_written) -> NTSTATUS {
        if (!target_address || !buffer || !bytes_written || size == 0) {
            return STATUS_UNSUCCESSFUL;
        }

        PHYSICAL_ADDRESS pa = { 0 };
        pa.QuadPart = (LONGLONG)target_address;

        PVOID mapped = MmMapIoSpaceEx(pa, size, PAGE_READWRITE);
        if (!mapped) {
            *bytes_written = 0;
            return STATUS_UNSUCCESSFUL;
        }

        __try {
            memcpy(mapped, buffer, size);
            *bytes_written = size;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            *bytes_written = 0;
            MmUnmapIoSpace(mapped, size);
            return STATUS_UNSUCCESSFUL;
        }

        MmUnmapIoSpace(mapped, size);
        return STATUS_SUCCESS;
    }

    UINT64 Cr3Cahce(UINT64 address, cache* cached_entry, SIZE_T* readsize) {
        if (cached_entry->Address == address) {
            return cached_entry->Value;
        }
        ReadMemory((address), &cached_entry->Value, sizeof(cached_entry->Value), readsize);
        cached_entry->Address = address;
        return cached_entry->Value;
    }

    auto TranslateLinear(UINT64 dtbase, UINT64 vtaddy) -> uint64_t {
        if (!dtbase) return 0;

        dtbase &= ~0xf;

        UINT64 pageOffset = vtaddy & ~(~0ull << PAGE_OFFSET_SIZE);
        UINT64 pte = (vtaddy >> 12) & 0x1ff;
        UINT64 pt = (vtaddy >> 21) & 0x1ff;
        UINT64 pd = (vtaddy >> 30) & 0x1ff;
        UINT64 pdp = (vtaddy >> 39) & 0x1ff;

        SIZE_T readsize = 0;
        UINT64 pdpe = 0;
        pdpe = Cr3Cahce(dtbase + 8 * pdp, &cached_pml4e[pdp], &readsize);
        if ((pdpe & 1) == 0)
            return 0;

        UINT64 pde = 0;
        ReadMemory(((pdpe & PMASK) + 8 * pd), &pde, sizeof(pde), &readsize);
        if ((pde & 1) == 0)
            return 0;

        if (pde & 0x80) {
            return (pde & (~0ull << 42 >> 12)) + (vtaddy & ~(~0ull << 30));
        }

        UINT64 pteAddr = 0;
        ReadMemory(((pde & PMASK) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
        if ((pteAddr & 1) == 0)
            return 0;

        if (pteAddr & 0x80) {
            return (pteAddr & PMASK) + (vtaddy & ~(~0ull << 21));
        }

        UINT64 finalAddr = 0;
        ReadMemory(((pteAddr & PMASK) + 8 * pte), &finalAddr, sizeof(finalAddr), &readsize);
        finalAddr &= PMASK;
        if (finalAddr == 0)
            return 0;

        return finalAddr + pageOffset;
    }

    auto translate(std::uint64_t dtb, std::uint64_t virtual_address, std::uint32_t* out_page_size) -> std::uint64_t {
        if (!dtb) {
            return 0;
        }

        auto va = virt_addr_t{};
        va.value = reinterpret_cast<void*>(virtual_address);

        if (va.pml4_index >= 512) {
            return 0;
        }

        pml4e pml4_entry{};
        SIZE_T bytes_read = 0;
        auto pml4_addr = (dtb & ~0xf) + (va.pml4_index * sizeof(pml4e));
        
        if (ReadMemory(pml4_addr, &pml4_entry, sizeof(pml4e), &bytes_read) != STATUS_SUCCESS) {
            return 0;
        }

        if (!pml4_entry.hard.present) {
            return 0;
        }

        pdpte pdpt_entry{};
        auto pdpt_addr = (pml4_entry.hard.pfn << page_shift) + (va.pdpt_index * sizeof(pdpte));
        
        if (ReadMemory(pdpt_addr, &pdpt_entry, sizeof(pdpte), &bytes_read) != STATUS_SUCCESS) {
            return 0;
        }

        if (!pdpt_entry.hard.present) {
            return 0;
        }

        if (pdpt_entry.hard.page_size) {
            auto final_pa = (pdpt_entry.hard.pfn << page_shift) + (virtual_address & page_1gb_mask);
            if (out_page_size)
                *out_page_size = page_1gb_size;
            return final_pa;
        }

        pde pd_entry{};
        auto pd_addr = (pdpt_entry.hard.pfn << page_shift) + (va.pd_index * sizeof(pde));
        
        if (ReadMemory(pd_addr, &pd_entry, sizeof(pde), &bytes_read) != STATUS_SUCCESS) {
            return 0;
        }

        if (!pd_entry.hard.present) {
            return 0;
        }

        if (pd_entry.hard.page_size) {
            auto final_pa = (pd_entry.hard.pfn << page_shift) + (virtual_address & page_2mb_mask);
            if (out_page_size)
                *out_page_size = page_2mb_size;
            return final_pa;
        }

        pte pt_entry{};
        auto pt_addr = (pd_entry.hard.pfn << page_shift) + (va.pt_index * sizeof(pte));
        
        if (ReadMemory(pt_addr, &pt_entry, sizeof(pte), &bytes_read) != STATUS_SUCCESS) {
            return 0;
        }

        if (!pt_entry.hard.present) {
            return 0;
        }

        auto final_pa = (pt_entry.hard.pfn << page_shift) + (virtual_address & page_4kb_mask);
        if (out_page_size)
            *out_page_size = page_4kb_size;
        return final_pa;
    }
}
