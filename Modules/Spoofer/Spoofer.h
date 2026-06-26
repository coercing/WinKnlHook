#pragma once
#include "../../Includes.h"

namespace Spoofer
{
    NTSTATUS Initialize();
    void Cleanup();

    // Hooked Dispatch Routines
    NTSTATUS HookedDiskDispatch(PDEVICE_OBJECT device, PIRP irp);
    NTSTATUS HookedNdisDispatch(PDEVICE_OBJECT device, PIRP irp);
    
    // Internal state
    inline bool g_Initialized = false;
}
