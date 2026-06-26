#pragma once
#include <ntifs.h>
#include <windef.h>
#include <cstdint>
#include <intrin.h>
#include "../../Includes.h"
#define FindMin(val1, val2) (static_cast<ULONG64>((val1) < (val2) ? (val1) : (val2)))
#define ToLower( chr ) ( ( chr >= ( 'A' ) && chr <= ( 'Z' ) ) ? ( chr + ( 32 ) ) : chr )
inline int strcmpp(const char* cs, const char* ct) {
    if (cs && ct) {
        while (*cs == *ct) {
            if (*cs == (0) && *ct == (0)) return (0);
            if (*cs == (0) || *ct == (0)) break;
            cs++;
            ct++;
        }

        return *cs - *ct;
    }

    return (-1);
}
typedef union _virt_addr_t
{
    void* value;
    struct
    {
        uintptr_t offset : 12;
        uintptr_t pt_index : 9;
        uintptr_t pd_index : 9;
        uintptr_t pdpt_index : 9;
        uintptr_t pml4_index : 9;
        uintptr_t reserved : 16;
    };
} virt_addr_t, * pvirt_addr_t;
typedef struct _MI_ACTIVE_PFN
{
    union
    {
        struct
        {
            struct
            {
                unsigned __int64 Tradable : 1;
                unsigned __int64 NonPagedBuddy : 43;
            };
        }  Leaf;
        struct
        {
            struct
            {
                unsigned __int64 Tradable : 1;
                unsigned __int64 WsleAge : 3;
                unsigned __int64 OldestWsleLeafEntries : 10;
                unsigned __int64 OldestWsleLeafAge : 3;
                unsigned __int64 NonPagedBuddy : 43;
            };
        }  PageTable;
        unsigned __int64 EntireActiveField;
    };
} MI_ACTIVE_PFN, * PMI_ACTIVE_PFN;
typedef struct _MMPTE_HARDWARE
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 Dirty1 : 1;
        unsigned __int64 Owner : 1;
        unsigned __int64 WriteThrough : 1;
        unsigned __int64 CacheDisable : 1;
        unsigned __int64 Accessed : 1;
        unsigned __int64 Dirty : 1;
        unsigned __int64 LargePage : 1;
        unsigned __int64 Global : 1;
        unsigned __int64 CopyOnWrite : 1;
        unsigned __int64 Unused : 1;
        unsigned __int64 Write : 1;
        unsigned __int64 PageFrameNumber : 40;
        unsigned __int64 ReservedForSoftware : 4;
        unsigned __int64 WsleAge : 4;
        unsigned __int64 WsleProtection : 3;
        unsigned __int64 NoExecute : 1;
    };
} MMPTE_HARDWARE, * PMMPTE_HARDWARE;
#define rva(addr, size)((PBYTE)(addr + *(DWORD*)(addr + ((size) - 4)) + size))
typedef struct _TAG_WND
{
    char padding_0[0x10];
    struct _TAG_INFO* tag_info;
    char padding_0x0[0x40];
    struct _TAG_WND* next; // 0x58
    struct _TAG_WND* prev; // 0x60
    struct _TAG_WND* parent; // 0x68
    struct _TAG_WND* child; // 0x70
}TAG_WND, * PTAG_WND;
struct comms_t {
    std::uint32_t key;

    struct {
        void* handle;
    } window;
};
typedef struct _MMPTE_PROTOTYPE
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 DemandFillProto : 1;
        unsigned __int64 HiberVerifyConverted : 1;
        unsigned __int64 ReadOnly : 1;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 Combined : 1;
        unsigned __int64 Unused1 : 4;
        __int64 ProtoAddress : 48;
    };
} MMPTE_PROTOTYPE, * PMMPTE_PROTOTYPE;
typedef struct _MMPTE_SOFTWARE
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 PageFileReserved : 1;
        unsigned __int64 PageFileAllocated : 1;
        unsigned __int64 ColdPage : 1;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 Transition : 1;
        unsigned __int64 PageFileLow : 4;
        unsigned __int64 UsedPageTableEntries : 10;
        unsigned __int64 ShadowStack : 1;
        unsigned __int64 Unused : 5;
        unsigned __int64 PageFileHigh : 32;
    };
} MMPTE_SOFTWARE, * PMMPTE_SOFTWARE;
typedef struct _MMPTE_TIMESTAMP
{
    struct
    {
        unsigned __int64 MustBeZero : 1;
        unsigned __int64 Unused : 3;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 Transition : 1;
        unsigned __int64 PageFileLow : 4;
        unsigned __int64 Reserved : 16;
        unsigned __int64 GlobalTimeStamp : 32;
    };
} MMPTE_TIMESTAMP, * PMMPTE_TIMESTAMP;
typedef struct _MMPTE_TRANSITION
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 Write : 1;
        unsigned __int64 Spare : 1;
        unsigned __int64 IoTracker : 1;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 Transition : 1;
        unsigned __int64 PageFrameNumber : 40;
        unsigned __int64 Unused : 12;
    };
} MMPTE_TRANSITION, * PMMPTE_TRANSITION;
typedef struct _MMPTE_SUBSECTION
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 Unused0 : 3;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 ColdPage : 1;
        unsigned __int64 Unused1 : 3;
        unsigned __int64 ExecutePrivilege : 1;
        __int64 SubsectionAddress : 48;
    };
} MMPTE_SUBSECTION, * PMMPTE_SUBSECTION;
typedef struct _MMPTE_LIST
{
    struct
    {
        unsigned __int64 Valid : 1;
        unsigned __int64 OneEntry : 1;
        unsigned __int64 filler0 : 2;
        unsigned __int64 SwizzleBit : 1;
        unsigned __int64 Protection : 5;
        unsigned __int64 Prototype : 1;
        unsigned __int64 Transition : 1;
        unsigned __int64 filler1 : 16;
        unsigned __int64 NextEntry : 36;
    };
} MMPTE_LIST, * PMMPTE_LIST;
typedef struct _MMPTE
{
    union
    {
        union
        {
            unsigned __int64 Long;
            volatile unsigned __int64 VolatileLong;
            struct _MMPTE_HARDWARE Hard;
            struct _MMPTE_PROTOTYPE Proto;
            struct _MMPTE_SOFTWARE Soft;
            struct _MMPTE_TIMESTAMP TimeStamp;
            struct _MMPTE_TRANSITION Trans;
            struct _MMPTE_SUBSECTION Subsect;
            struct _MMPTE_LIST List;
        };
    }  u;
} MMPTE, * PMMPTE;
typedef struct _MIPFNBLINK
{
    union
    {
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 Blink : 40; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 NodeBlinkLow : 19; /* bit position: 40 */
            /* 0x0000 */ unsigned __int64 TbFlushStamp : 3; /* bit position: 59 */
            /* 0x0000 */ unsigned __int64 PageBlinkDeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 PageBlinkLockBit : 1; /* bit position: 63 */
        }; /* bitfield */
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 ShareCount : 62; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 PageShareCountDeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 PageShareCountLockBit : 1; /* bit position: 63 */
        }; /* bitfield */
        /* 0x0000 */ unsigned __int64 EntireField;
        /* 0x0000 */ volatile __int64 Lock;
        struct /* bitfield */
        {
            /* 0x0000 */ unsigned __int64 LockNotUsed : 62; /* bit position: 0 */
            /* 0x0000 */ unsigned __int64 DeleteBit : 1; /* bit position: 62 */
            /* 0x0000 */ unsigned __int64 LockBit : 1; /* bit position: 63 */
        }; /* bitfield */
    }; /* size: 0x0008 */
} MIPFNBLINK, * PMIPFNBLINK; /* size: 0x0008 */
typedef struct _MMPFNENTRY1
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned char PageLocation : 3; /* bit position: 0 */
        /* 0x0000 */ unsigned char WriteInProgress : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned char Modified : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned char ReadInProgress : 1; /* bit position: 5 */
        /* 0x0000 */ unsigned char CacheAttribute : 2; /* bit position: 6 */
    }; /* bitfield */
} MMPFNENTRY1, * PMMPFNENTRY1; /* size: 0x0001 */
typedef struct _MMPFNENTRY3
{
    struct /* bitfield */
    {
        /* 0x0000 */ unsigned char Priority : 3; /* bit position: 0 */
        /* 0x0000 */ unsigned char OnProtectedStandby : 1; /* bit position: 3 */
        /* 0x0000 */ unsigned char InPageError : 1; /* bit position: 4 */
        /* 0x0000 */ unsigned char SystemChargedPage : 1; /* bit position: 5 */
        /* 0x0000 */ unsigned char RemovalRequested : 1; /* bit position: 6 */
        /* 0x0000 */ unsigned char ParityError : 1; /* bit position: 7 */
    }; /* bitfield */
} MMPFNENTRY3, * PMMPFNENTRY3; /* size: 0x0001 */
typedef struct _MI_PFN_ULONG5
{
    union
    {
        /* 0x0000 */ unsigned long EntireField;
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned long NodeBlinkHigh : 21; /* bit position: 0 */
                /* 0x0000 */ unsigned long NodeFlinkMiddle : 11; /* bit position: 21 */
            }; /* bitfield */
        } /* size: 0x0004 */ StandbyList;
        struct
        {
            /* 0x0000 */ unsigned char ModifiedListBucketIndex : 4; /* bit position: 0 */
        } /* size: 0x0001 */ MappedPageList;
        struct
        {
            struct /* bitfield */
            {
                /* 0x0000 */ unsigned char AnchorLargePageSize : 2; /* bit position: 0 */
                /* 0x0000 */ unsigned char Spare1 : 6; /* bit position: 2 */
            }; /* bitfield */
            /* 0x0001 */ unsigned char ViewCount;
            /* 0x0002 */ unsigned short Spare2;
        } /* size: 0x0004 */ Active;
    }; /* size: 0x0004 */
} MI_PFN_ULONG5, * PMI_PFN_ULONG5; /* size: 0x0004 */
typedef struct _MMPFN
{
    union
    {
        /* 0x0000 */ struct _LIST_ENTRY ListEntry;
        /* 0x0000 */ struct _RTL_BALANCED_NODE TreeNode;
        struct
        {
            union
            {
                union
                {
                    /* 0x0000 */ struct _SINGLE_LIST_ENTRY NextSlistPfn;
                    /* 0x0000 */ void* Next;
                    struct /* bitfield */
                    {
                        /* 0x0000 */ unsigned __int64 Flink : 40; /* bit position: 0 */
                        /* 0x0000 */ unsigned __int64 NodeFlinkLow : 24; /* bit position: 40 */
                    }; /* bitfield */
                    /* 0x0000 */ struct _MI_ACTIVE_PFN Active;
                }; /* size: 0x0008 */
            } /* size: 0x0008 */ u1;
            union
            {
                /* 0x0008 */ struct _MMPTE* PteAddress;
                /* 0x0008 */ unsigned __int64 PteLong;
            }; /* size: 0x0008 */
            /* 0x0010 */ struct _MMPTE OriginalPte;
        }; /* size: 0x0018 */
    }; /* size: 0x0018 */
    /* 0x0018 */ struct _MIPFNBLINK u2;
    union
    {
        union
        {
            struct
            {
                /* 0x0020 */ unsigned short ReferenceCount;
                /* 0x0022 */ struct _MMPFNENTRY1 e1;
                /* 0x0023 */ struct _MMPFNENTRY3 e3;
            }; /* size: 0x0004 */
            struct
            {
                /* 0x0020 */ unsigned short ReferenceCount;
            } /* size: 0x0002 */ e2;
            struct
            {
                /* 0x0020 */ unsigned long EntireField;
            } /* size: 0x0004 */ e4;
        }; /* size: 0x0004 */
    } /* size: 0x0004 */ u3;
    /* 0x0024 */ struct _MI_PFN_ULONG5 u5;
    union
    {
        union
        {
            struct /* bitfield */
            {
                /* 0x0028 */ unsigned __int64 PteFrame : 40; /* bit position: 0 */
                /* 0x0028 */ unsigned __int64 ResidentPage : 1; /* bit position: 40 */
                /* 0x0028 */ unsigned __int64 Unused1 : 1; /* bit position: 41 */
                /* 0x0028 */ unsigned __int64 Unused2 : 1; /* bit position: 42 */
                /* 0x0028 */ unsigned __int64 Partition : 10; /* bit position: 43 */
                /* 0x0028 */ unsigned __int64 FileOnly : 1; /* bit position: 53 */
                /* 0x0028 */ unsigned __int64 PfnExists : 1; /* bit position: 54 */
                /* 0x0028 */ unsigned __int64 NodeFlinkHigh : 5; /* bit position: 55 */
                /* 0x0028 */ unsigned __int64 PageIdentity : 3; /* bit position: 60 */
                /* 0x0028 */ unsigned __int64 PrototypePte : 1; /* bit position: 63 */
            }; /* bitfield */
            /* 0x0028 */ unsigned __int64 EntireField;
        }; /* size: 0x0008 */
    } /* size: 0x0008 */ u4;
} MMPFN, * PMMPFN; /* size: 0x0030 */
struct cache {
    uintptr_t Address;
    UINT64 Value;
};
typedef enum SYSTEM_INFORMATION_CLASS
{
    SystemInformationClassMin = 0,
    SystemBasicInformation = 0,
    SystemProcessorInformation = 1,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3,
    SystemPathInformation = 4,
    SystemNotImplemented1 = 4,
    SystemProcessInformation = 5,
    SystemProcessesAndThreadsInformation = 5,
    SystemCallCountInfoInformation = 6,
    SystemCallCounts = 6,
    SystemDeviceInformation = 7,
    SystemConfigurationInformation = 7,
    SystemProcessorPerformanceInformation = 8,
    SystemProcessorTimes = 8,
    SystemFlagsInformation = 9,
    SystemGlobalFlag = 9,
    SystemCallTimeInformation = 10,
    SystemNotImplemented2 = 10,
    SystemModuleInformation = 11,
    SystemLocksInformation = 12,
    SystemLockInformation = 12,
    SystemStackTraceInformation = 13,
    SystemNotImplemented3 = 13,
    SystemPagedPoolInformation = 14,
    SystemNotImplemented4 = 14,
    SystemNonPagedPoolInformation = 15,
    SystemNotImplemented5 = 15,
    SystemHandleInformation = 16,
    SystemObjectInformation = 17,
    SystemPageFileInformation = 18,
    SystemPagefileInformation = 18,
    SystemVdmInstemulInformation = 19,
    SystemInstructionEmulationCounts = 19,
    SystemVdmBopInformation = 20,
    SystemInvalidInfoClass1 = 20,
    SystemFileCacheInformation = 21,
    SystemCacheInformation = 21,
    SystemPoolTagInformation = 22,
    SystemInterruptInformation = 23,
    SystemProcessorStatistics = 23,
    SystemDpcBehaviourInformation = 24,
    SystemDpcInformation = 24,
    SystemFullMemoryInformation = 25,
    SystemNotImplemented6 = 25,
    SystemLoadImage = 26,
    SystemUnloadImage = 27,
    SystemTimeAdjustmentInformation = 28,
    SystemTimeAdjustment = 28,
    SystemSummaryMemoryInformation = 29,
    SystemNotImplemented7 = 29,
    SystemNextEventIdInformation = 30,
    SystemNotImplemented8 = 30,
    SystemEventIdsInformation = 31,
    SystemNotImplemented9 = 31,
    SystemCrashDumpInformation = 32,
    SystemExceptionInformation = 33,
    SystemCrashDumpStateInformation = 34,
    SystemKernelDebuggerInformation = 35,
    SystemContextSwitchInformation = 36,
    SystemRegistryQuotaInformation = 37,
    SystemLoadAndCallImage = 38,
    SystemPrioritySeparation = 39,
    SystemPlugPlayBusInformation = 40,
    SystemNotImplemented10 = 40,
    SystemDockInformation = 41,
    SystemNotImplemented11 = 41,
    SystemInvalidInfoClass2 = 42,
    SystemProcessorSpeedInformation = 43,
    SystemInvalidInfoClass3 = 43,
    SystemCurrentTimeZoneInformation = 44,
    SystemTimeZoneInformation = 44,
    SystemLookasideInformation = 45,
    SystemSetTimeSlipEvent = 46,
    SystemCreateSession = 47,
    SystemDeleteSession = 48,
    SystemInvalidInfoClass4 = 49,
    SystemRangeStartInformation = 50,
    SystemVerifierInformation = 51,
    SystemAddVerifier = 52,
    SystemSessionProcessesInformation = 53,
    SystemInformationClassMax
} SYSTEM_INFORMATION_CLASS;
typedef struct _KLDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;       // 0x0
    LIST_ENTRY InMemoryOrderLinks;     // optional if needed
    LIST_ENTRY InInitializationOrderLinks; // optional if needed
    PVOID DllBase;                     // base address of module
    PVOID EntryPoint;                  // entry point
    ULONG SizeOfImage;                 // module image size
    UNICODE_STRING FullDllName;        // full path
    UNICODE_STRING BaseDllName;        // module file name
    ULONG Flags;                       // flags
    SHORT LoadCount;                   // number of loads
    SHORT TlsIndex;                    // TLS index
    LIST_ENTRY HashLinks;              // optional
    ULONG TimeDateStamp;               // timestamp
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;
typedef struct rtl_process_module_information
{
    HANDLE section;
    PVOID mapped_base;
    PVOID image_base;
    ULONG image_size;
    ULONG flags;
    USHORT load_order_index;
    USHORT init_order_index;
    USHORT load_count;
    USHORT offset_to_file_name;
    UCHAR full_path_name[256];
} rtl_process_module_information, * prtl_process_module_information;
typedef struct rtl_process_modules
{
    ULONG number_of_modules;
    rtl_process_module_information modules[1];
} rtl_process_modules, * prtl_process_modules;
extern "C" __declspec(dllimport) void* IoDriverObjectType;

inline ULONG g_cached_pool_tag = 0;

static const UINT64 PMASK = (~0xfull << 8) & 0xfffffffffull;
#define CONTAINING_RECORD(address, type, field) \
    ((type *)((char*)(address) - (uintptr_t)(&((type *)0)->field)))

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT TlsIndex;
    union {
        LIST_ENTRY HashLinks;
        struct {
            PVOID SectionPointer;
            ULONG CheckSum;
        };
    };
    union {
        struct {
            ULONG TimeDateStamp;
        };
        struct {
            PVOID LoadedImports;
        };
    };
    PVOID* EntryPointActivationContext;

    PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;
//#define KeAcquireSpinLocknew(SpinLock, OldIrql) \
//    *(OldIrql) = imports.KeAcquireSpinLockRaiseToDpc(SpinLock)

typedef struct
{
    ULONG length;
    UCHAR initialized;
    PVOID ss_handle;
    LIST_ENTRY in_load_order_module_list;
    LIST_ENTRY in_memory_order_module_list;
    LIST_ENTRY in_initialization_order_module_list;
} peb_ldr_data, * ppeb_ldr_data;

extern "C" {
#pragma warning( push )
#pragma warning( disable : 4201 )
#define NAMELESS_DUMMY_UNION



#pragma warning( pop )

    typedef VOID
    (*MouseClassServiceCallbackFn)(
        PDEVICE_OBJECT DeviceObject,
        PMOUSE_INPUT_DATA InputDataStart,
        PMOUSE_INPUT_DATA InputDataEnd,
        PULONG InputDataConsumed
        );

    typedef struct _MOUSE_OBJECT
    {
        PDEVICE_OBJECT              DeviceObject;
        MouseClassServiceCallbackFn ServiceCallback;
        BOOLEAN                     UseMouse;
    } MOUSE_OBJECT, * PMOUSE_OBJECT;

    //
    // Forward ASM Function
    // 

    VOID MouseClassServiceCallback(
        PDEVICE_OBJECT DeviceObject,
        PMOUSE_INPUT_DATA InputDataStart,
        PMOUSE_INPUT_DATA InputDataEnd,
        PULONG InputDataConsumed
    );

}
inline MOUSE_OBJECT gMouseObject;
extern "C" {
    inline ULONG64 _KeAcquireSpinLockAtDpcLevel;
    inline ULONG64 _KeReleaseSpinLockFromDpcLevel;
    inline ULONG64 _IofCompleteRequest;
    inline ULONG64 _IoReleaseRemoveLockEx;
}
extern "C"
{

    EXTERN_C PLIST_ENTRY PsLoadedModuleList;
    uintptr_t get_nt_base();
    NTKERNELAPI PVOID NTAPI RtlFindExportedRoutineByName(PVOID, PCCH);

}
