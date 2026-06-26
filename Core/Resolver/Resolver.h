#pragma once
#include "../../includes.h"

class C_Resolver {
public:

    inline bool SetupASM() {
        // Try ASM method first
        this->m_nt_base = get_nt_base();
        
        // Fallback to manual search if ASM fails
        if (this->m_nt_base == 0) {
            DbgPrintEx(0, 0, "[-] sebemu: asm get_nt_base failed, using fallback\n");
            this->m_nt_base = GetNtBaseFallback();
        }
        
        if (this->m_nt_base != 0) {
            DbgPrintEx(0, 0, "[+] sebemu: ntoskrnl.exe base = 0x%p\n", (void*)this->m_nt_base);
        }
        
        return (this->m_nt_base != 0);
    }

    inline uintptr_t GetSystemRoutine(const char* export_name) const {

        auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(m_nt_base);
        auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(m_nt_base + dos->e_lfanew);

        if (dos->e_magic != IMAGE_DOS_SIGNATURE)
            return 0;

        if (nt->Signature != IMAGE_NT_SIGNATURE)
            return 0;

        auto exp_dir = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
            m_nt_base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
            );

        if (!exp_dir->AddressOfFunctions ||
            !exp_dir->AddressOfNames ||
            !exp_dir->AddressOfNameOrdinals)
            return 0;

        auto names = (uint32_t*)(m_nt_base + exp_dir->AddressOfNames);
        auto funcs = (uint32_t*)(m_nt_base + exp_dir->AddressOfFunctions);
        auto ords = (uint16_t*)(m_nt_base + exp_dir->AddressOfNameOrdinals);

        for (uint32_t i = 0; i < exp_dir->NumberOfNames; i++) {
            const char* cur_name = (const char*)(m_nt_base + names[i]);
            uintptr_t cur_func = m_nt_base + funcs[ords[i]];

            if (crt::strcmp(export_name, cur_name) == 0)
                return cur_func;
        }

        return 0;
    }

private:
    uintptr_t m_nt_base = 0;
    
    // Fallback method using MmGetSystemRoutineAddress
    inline uintptr_t GetNtBaseFallback() {
        UNICODE_STRING routineName;
        RtlInitUnicodeString(&routineName, L"MmGetSystemRoutineAddress");
        
        // Get any known kernel export to find base
        PVOID routine = MmGetSystemRoutineAddress(&routineName);
        if (!routine) {
            return 0;
        }
        
        // Walk backwards to find PE header
        uintptr_t addr = reinterpret_cast<uintptr_t>(routine);
        addr &= ~0xFFF; // Align to page boundary
        
        for (int i = 0; i < 0x10000; i++) {
            addr -= 0x1000;
            
            __try {
                auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(addr);
                if (dos->e_magic == IMAGE_DOS_SIGNATURE) {
                    auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(addr + dos->e_lfanew);
                    if (nt->Signature == IMAGE_NT_SIGNATURE) {
                        return addr;
                    }
                }
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                continue;
            }
        }
        
        return 0;
    }

}; inline C_Resolver Resolver;
