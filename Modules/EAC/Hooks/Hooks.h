#pragma once
#include "../../../Includes.h"



namespace EACHooks {
    NTSTATUS setup();
    NTSTATUS BlockEACFunctions();
    void UninstallHooks();
    typedef int(__cdecl* _vsnwprintf_t)(wchar_t*, size_t, const wchar_t*, va_list);
    inline _vsnwprintf_t g_Orig_vsnwprintf = nullptr;
    typedef BOOLEAN(NTAPI* KdEnteredDebugger_t)(VOID);
    inline KdEnteredDebugger_t g_Orig_KdEnteredDebugger = nullptr;

    inline uintptr_t EAC_Base = 0;
    inline uintptr_t DecreptImports = 0;
    inline uintptr_t mmCopyMemoryCallSite = 0;
    inline uintptr_t psGetProcessSectionBaseAddressCallSite = 0;
    inline uintptr_t psInitialSystemProcessCallSite = 0;
    inline uintptr_t mmIsAddressValidCallSite = 0;
    inline uintptr_t psGetProcessIdCallSite = 0;

    inline uintptr_t Protect_Memory = 0;
    inline uintptr_t Allocate_Memory = 0;
    inline uintptr_t Zero_Memory = 0;
    inline uintptr_t Free_Memory = 0;
    inline uintptr_t Attach_Memory = 0;
    inline uintptr_t Detach_Memory = 0;

}
