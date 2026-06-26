#include "pml4.h"

namespace PML4 {
void *KDb = nullptr;

PVOID fpim(const void *startAddress, size_t memorySize, const void *pattern,
           size_t patternSize) {
  const auto *memoryStart = static_cast<const uint8_t *>(startAddress);
  const auto *memoryPattern = static_cast<const uint8_t *>(pattern);

  for (size_t offset = 0; offset <= memorySize - patternSize; ++offset) {
    size_t patternIndex = 0;
    while (patternIndex < patternSize &&
           memoryStart[offset + patternIndex] == memoryPattern[patternIndex]) {
      ++patternIndex;
    }
    if (patternIndex == patternSize) {
      return const_cast<uint8_t *>(&memoryStart[offset]);
    }
  }
  return nullptr;
}
NTSTATUS initDb() {
  struct PfnDatabasePattern {
    const uint8_t *bytePattern;
    size_t byteSize;
    bool isHardcoded;
  };

  static const uint8_t win10X64Pattern[] = {0x48, 0x8B, 0xC1, 0x48, 0xC1, 0xE8,
                                            0x0C, 0x48, 0x8D, 0x14, 0x40, 0x48,
                                            0x03, 0xD2, 0x48, 0xB8};

  PfnDatabasePattern searchConfig{win10X64Pattern, sizeof(win10X64Pattern),
                                  true};
  auto *virtualFunctionAddress = reinterpret_cast<uint8_t *>(
      Resolver.GetSystemRoutine("MmGetVirtualForPhysical"));
  if (!virtualFunctionAddress) {
    return STATUS_PROCEDURE_NOT_FOUND;
  }

  auto *resultAddress = reinterpret_cast<uint8_t *>(
      fpim(virtualFunctionAddress, 0x20, searchConfig.bytePattern,
           searchConfig.byteSize));
  if (!resultAddress) {
    return STATUS_UNSUCCESSFUL;
  }

  resultAddress += searchConfig.byteSize;

  if (searchConfig.isHardcoded) {
    KDb = *reinterpret_cast<void **>(resultAddress);
  } else {
    auto pfnAddress = *reinterpret_cast<uintptr_t *>(resultAddress);
    KDb = *reinterpret_cast<void **>(pfnAddress);
  }

  KDb = PAGE_ALIGN(KDb);
  return STATUS_SUCCESS;
}
static auto DirbaseFromBaseAddress(void *processBase) -> uint64_t {
  if (!NT_SUCCESS(initDb())) {
    return 0;
  }

  virt_addr_t virtualBase{};
  virtualBase.value = processBase;

  auto physicalMemory = (NT.MmGetPhysicalMemoryRanges)();
  if (!physicalMemory) {
    return 0;
  }

  for (int i = 0; physicalMemory[i].BaseAddress.QuadPart; ++i) {
    auto &currentRange = physicalMemory[i];
    uint64_t physicalAddr = currentRange.BaseAddress.QuadPart;

    for (uint64_t offset = 0; offset < currentRange.NumberOfBytes.QuadPart;
         offset += 0x1000, physicalAddr += 0x1000) {
      auto pfn = reinterpret_cast<_MMPFN *>(
          (uintptr_t)KDb + ((physicalAddr >> 12) * sizeof(_MMPFN)));

      if (pfn->u4.PteFrame == (physicalAddr >> 12)) {
        size_t bytesRead = 0;
        MMPTE pml4Entry{};
        if (!NT_SUCCESS(
                RWX::ReadMemory((physicalAddr + 8 * virtualBase.pml4_index),
                                &pml4Entry, 8, &bytesRead))) {
          continue;
        }
        if (!pml4Entry.u.Hard.Valid) {
          continue;
        }

        MMPTE pdptEntry{};
        if (!NT_SUCCESS(
                RWX::ReadMemory(((pml4Entry.u.Hard.PageFrameNumber << 12) +
                                 8 * virtualBase.pdpt_index),
                                &pdptEntry, 8, &bytesRead))) {
          continue;
        }
        if (!pdptEntry.u.Hard.Valid) {
          continue;
        }

        MMPTE pdEntry{};
        if (!NT_SUCCESS(
                RWX::ReadMemory(((pdptEntry.u.Hard.PageFrameNumber << 12) +
                                 8 * virtualBase.pd_index),
                                &pdEntry, 8, &bytesRead))) {
          continue;
        }
        if (!pdEntry.u.Hard.Valid) {
          continue;
        }

        MMPTE ptEntry{};
        if (!NT_SUCCESS(
                RWX::ReadMemory(((pdEntry.u.Hard.PageFrameNumber << 12) +
                                 8 * virtualBase.pt_index),
                                &ptEntry, 8, &bytesRead))) {
          continue;
        }
        if (!ptEntry.u.Hard.Valid) {
          continue;
        }

        return physicalAddr;
      }
    }
  }
  return 0;
}

NTSTATUS DecryptCR3(pdtb request) {
  ULONGLONG cr3_value =
      DirbaseFromBaseAddress(reinterpret_cast<void *>(request->BaseAddress));
  if (!cr3_value) {
    return STATUS_UNSUCCESSFUL;
  }

  // Clear the PML4 cache when CR3 changes
  if (suDB != cr3_value) {
    RtlZeroMemory(RWX::cached_pml4e, sizeof(RWX::cached_pml4e));
  }

  suDB = cr3_value;
  *reinterpret_cast<ULONGLONG *>(request->outaddress) = cr3_value;
  return STATUS_SUCCESS;
}

} // namespace PML4