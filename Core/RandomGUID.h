#pragma once
#include "../Includes.h"

// Shared GUID storage between KM and UM (in-memory only)
namespace RandomGUID
{
    // GUID placeholder pattern - will be replaced by user-mode before mapping
    // Pattern: 0xDEADBEEF repeated 4 times (16 bytes total for GUID)
    // IMPORTANT: This must be aligned and in .data section (not .rdata)
    // Force into writable data section to prevent page faults
    #pragma section(".data", read, write)
    #pragma comment(linker, "/SECTION:.data,RW")
    __declspec(allocate(".data")) __declspec(align(16)) GUID CurrentSessionGUID = { 0xDEADBEEF, 0xDEAD, 0xBEEF, {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF} };

    // Get current session GUID (safe kernel-mode access)
    inline GUID GetSessionGUID()
    {
        GUID result;
        
        // Use __try/__except to catch any access violations
        __try {
            // Direct structure copy (safer than RtlCopyMemory for aligned data)
            result.Data1 = CurrentSessionGUID.Data1;
            result.Data2 = CurrentSessionGUID.Data2;
            result.Data3 = CurrentSessionGUID.Data3;
            for (int i = 0; i < 8; i++) {
                result.Data4[i] = CurrentSessionGUID.Data4[i];
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            // If access fails, return the placeholder
            result.Data1 = 0xDEADBEEF;
            result.Data2 = 0xDEAD;
            result.Data3 = 0xBEEF;
            for (int i = 0; i < 8; i++) {
                result.Data4[i] = 0xDE;
            }
        }
        
        return result;
    }

    // Check if GUID is initialized (not placeholder)
    inline bool IsInitialized()
    {
        __try {
            return (CurrentSessionGUID.Data1 != 0xDEADBEEF || CurrentSessionGUID.Data2 != 0xDEAD);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }
}
