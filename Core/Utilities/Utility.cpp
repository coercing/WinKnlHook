#include "Utility.h"


namespace Utility
{
	void CustomMemcpy(const void* dstp, const void* srcp, UINT len)
	{
		ULONG* dst = (ULONG*)dstp;
		ULONG* src = (ULONG*)srcp;
		UINT i, tail;

		for (i = 0; i < (len / sizeof(ULONG)); i++)
			*dst++ = *src++;

		tail = len & (sizeof(ULONG) - 1);
		if (tail) {

			UCHAR* dstb = (UCHAR*)dstp;
			UCHAR* srcb = (UCHAR*)srcp;

			for (i = len - tail; i < len; i++)
				dstb[i] = srcb[i];
		}
	}

    bool isBackedByModule(const wchar_t* module_name, void* address)
    {
        if (!module_name || !address)
            return false;

        size_t len = crt::wcslen(module_name);

        auto ps_loaded_module_list = reinterpret_cast<_KLDR_DATA_TABLE_ENTRY*>(
            Resolver.GetSystemRoutine(E("PsLoadedModuleList"))
            );

        if (!ps_loaded_module_list)
            return false;

        for (PLIST_ENTRY pListEntry = ps_loaded_module_list->InLoadOrderLinks.Flink;
            pListEntry != &ps_loaded_module_list->InLoadOrderLinks;
            pListEntry = pListEntry->Flink)
        {
            auto entry = CONTAINING_RECORD(pListEntry, _KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (entry->BaseDllName.Length / sizeof(wchar_t) == len &&
                memcmp(entry->BaseDllName.Buffer, module_name, len * sizeof(wchar_t)) == 0)
            {
                ULONG64 start = (ULONG64)entry->DllBase;
                ULONG64 end = start + entry->SizeOfImage;

                if ((ULONG64)address >= start && (ULONG64)address < end)
                    return true;
            }
        }

        return false;
    }

    int GetWindowsBuildNumber()
    {
        RTL_OSVERSIONINFOW ver{};
        ver.dwOSVersionInfoSize = sizeof(ver);

        if (!NT_SUCCESS(RtlGetVersion(&ver)))
            return 0;

        return ver.dwBuildNumber;
    }

    uintptr_t GetNToskrnlBase() {
        auto base = NT.GetModuleBase(E(L"ntoskrnl.exe"));
        printf("[Utility] ntoskrnl base = 0x%p\n", (void*)base);
        return base;

    }

    uint64_t InitRandom()
    {
        uintptr_t kernel_base = GetNToskrnlBase();

        uint32_t raw_tag32 =
            kernel_base
            ? static_cast<uint32_t>((kernel_base) & 0xFFFFFFFF)
            : 0x12345678u;

        uint32_t tag32 = (raw_tag32 & 0x7F7F7F7F) | 0x20202020;

        if (tag32 == 0 || tag32 == 0xFFFFFFFF)
            tag32 = 'ScHM';

        return static_cast<uint64_t>(tag32);

    }

    ULONG GetRandomPoolTag()
    {
        if (!g_cached_pool_tag) {
            g_cached_pool_tag = 'ScHM';
        }
        return g_cached_pool_tag;
    }


#define ExFreePoolnew(a) NT.ExFreePoolWithTag(a, GetRandomPoolTag())

    uintptr_t GetDebugPrintCallbackListOffset()
    {
        int build = GetWindowsBuildNumber();
        printf("[Utility] Windows build = %d\n", build);

        // Windows 11
        if (build >= 22000) {
            printf("[Utility] Using Windows 11 callback offset\n");
            return 0xC04260;
        }

        // Windows 10
        printf("[Utility] Using Windows 10 callback offset\n");
        return 0xC02B60;
    }

    LIST_ENTRY* GetDebugPrintCallbackList(uintptr_t ntoskrnl_base)
    {
        printf("[Utility] Resolving DebugPrintCallbackList...\n");

        uintptr_t offset = GetDebugPrintCallbackListOffset();
        if (!offset) {
            printf("[Utility] ERROR: offset is zero\n");
            return nullptr;
        }

        auto ptr = reinterpret_cast<LIST_ENTRY*>(ntoskrnl_base + offset);
        printf("[Utility] DebugPrintCallbackList = 0x%p (base 0x%p + offset 0x%llX)\n",
            ptr, (void*)ntoskrnl_base, offset);

        return ptr;
    }

    uintptr_t FindPattern(uintptr_t base, size_t size, const char* pattern, const char* mask) {
        size_t pattern_length = crt::strlen(mask);

        if (pattern_length == 0 || size < pattern_length)
            return 0;

        for (size_t i = 0; i <= size - pattern_length; ++i) {
            bool found = true;

            for (size_t j = 0; j < pattern_length; ++j) {
                if (mask[j] == 'x' &&
                    *(unsigned char*)(base + i + j) != (unsigned char)pattern[j]) {
                    found = false;
                    break;
                }
            }

            if (found)
                return base + i;
        }
        return 0;
    }

    uintptr_t FindPattern(uint64_t base, const char* pattern, const char* mask) {
        auto dos_header{ reinterpret_cast<_IMAGE_DOS_HEADER*> (base) };
        auto nt_headers{ reinterpret_cast<IMAGE_NT_HEADERS64*> (base + dos_header->e_lfanew) };

        auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
        auto result = FindPattern(base, size_of_image, pattern, mask);
        if (!result) {
        }
        return result;
    }
    
    uint64_t GetEacModule(const wchar_t* module_name) {

        UNICODE_STRING module_name_string{ };
        NT.RtlInitUnicodeString(&module_name_string, module_name);
        
        auto ps_loaded_module_list = reinterpret_cast<LIST_ENTRY*>(
            Resolver.GetSystemRoutine("PsLoadedModuleList")
            );
        if (!ps_loaded_module_list) {
            return 0;
        }
        
        auto iter_ldr_entry = reinterpret_cast<_KLDR_DATA_TABLE_ENTRY*>(
            ps_loaded_module_list->Flink
            );

        while (reinterpret_cast<LIST_ENTRY*>(iter_ldr_entry) != ps_loaded_module_list) {
            if (!NT.RtlCompareUnicodeString(&iter_ldr_entry->BaseDllName, &module_name_string, true)) {
                auto base = reinterpret_cast<std::uintptr_t>(iter_ldr_entry->DllBase);
                return base;
            }

            iter_ldr_entry = reinterpret_cast<_KLDR_DATA_TABLE_ENTRY*>(
                iter_ldr_entry->InLoadOrderLinks.Flink
                );
        }

        return 0;
    }

    void* GetSystemInfortmantion(SYSTEM_INFORMATION_CLASS information_class)
    {
        ULONG size = 0;
        NTSTATUS status = NT.ZwQuerySystemInformation(information_class, NULL, 0, &size);
        if (status != STATUS_INFO_LENGTH_MISMATCH && !size) {
            return nullptr;
        }

        void* info = NT.ExAllocatePoolWithTag(NonPagedPool, size, GetRandomPoolTag());
        if (!info) return nullptr;

        status = NT.ZwQuerySystemInformation(information_class, info, size, &size);
        if (!NT_SUCCESS(status)) {
            ExFreePooll(info);
            return nullptr;
        }

        return info;
    }
    
    uintptr_t GetKernelModule(const char* name)
    {
        const auto to_lower = [](char* string) -> const char* {
            for (char* pointer = string; *pointer != '\0'; ++pointer) {
                *pointer = (char)(short)ToLower(*pointer);
            }

            return string;
            };

        const prtl_process_modules info = (prtl_process_modules)GetSystemInfortmantion(SystemModuleInformation);

        if (!info)
            return NULL;

        for (size_t i = 0; i < info->number_of_modules; ++i) {
            const auto& mod = info->modules[i];

            if (strcmpp(to_lower((char*)mod.full_path_name + mod.offset_to_file_name), name) == 0) {
                const void* address = mod.image_base;
                ExFreePoolnew(info);
                return (uintptr_t)address;
            }
        }

        ExFreePoolnew(info);
        return NULL;
    }

    bool WriteSafe(void* address, void* buffer, const size_t size)
    {
        auto* mdl = NT.IoAllocateMdl(address, static_cast<ULONG>(size), FALSE, FALSE, nullptr);
        if (!mdl)
            return false;
        NT.MmProbeAndLockPages(mdl, KernelMode, IoReadAccess);
        auto* const mapping = NT.MmMapLockedPagesSpecifyCache(mdl, KernelMode, MmNonCached, nullptr, FALSE, NormalPagePriority);
        NT.MmProtectMdlSystemAddress(mdl, PAGE_READWRITE);
        memcpy(mapping, buffer, size);
        NT.MmUnmapLockedPages(mapping, mdl);
        NT.MmUnlockPages(mdl);
        NT.IoFreeMdl(mdl);
        return true;
    }


}