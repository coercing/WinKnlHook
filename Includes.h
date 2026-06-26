#pragma once

// Suppress all warnings first
#include "Core/Definitions/WarningSuppress.h"

//
// Windows & Kernel Headers
//
#include <ntifs.h>
#include <ntddmou.h>
#include <cstdint>
#include <ntimage.h>
#include <ntstrsafe.h>

//
// Global UNICODE_STRINGs
//
inline UNICODE_STRING DeviceN, DosL;

//
// Driver Core
//
#include "Core/Definitions/Define.h"
#include "Core/Definitions/Encrypt.h"
#include "Core/Definitions/IoCodes.h"
#include "Core/Crt/CRT.h"
#include "Core/Resolver/Resolver.h"
#include "Core/Symbols/Symbols.h"

inline ntkrnl NT;

#include "Core/Utilities/Utility.h"

//
// EAC Functions
//
#include "Modules/EAC/Hooks/Hooks.h"
#include "Modules/EAC/Decryption/Decryption.h"
#include "Modules/EAC/Functions/Functions.h"
#include "Modules/EAC/LogDisable/Log.h"


//
// Memory Functions
//
#include "Modules/Memory/EProcess/EProcess.h"
#include "Modules/Memory/BaseAddress/Base.h"
#include "Modules/MemoryAccess/RWX/RWX.h"
#include "Modules/Memory/PML4/PML4.h"

// 
// MemoryAccess Functions
//
#include "Modules/MemoryAccess/ReadHandler/Read.h"
#include "Modules/MemoryAccess/WriteHandler/Write.h"

//
// Pattern Scan Functions
//
#include "Modules/PatternScan/PatternScan.h"

//
// Mouse Move Functions
// 
#include "Modules/MouseMove/MouseMove.h"

// Driver IoControl
//
#include "Core/IoControl/IoControl.h"


//
// Driver Initialization
//
#include "Init/DriverInit/DriverInit.h"
