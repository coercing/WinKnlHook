#pragma once
#include "../../includes.h"

class ntkrnl
{
public:

    inline PVOID RtlLookupFunctionEntry(DWORD64 ControlPc, PDWORD64 ImageBase, PVOID HistoryTable)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("RtlLookupFunctionEntry");
        if (!function_address) return nullptr;
        using Fn = PVOID(*)(DWORD64, PDWORD64, PVOID);
        return reinterpret_cast<Fn>(function_address)(ControlPc, ImageBase, HistoryTable);
    }

    inline NTSTATUS NtQueryValueKey(HANDLE KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass, PVOID KeyValueInformation, ULONG Length, PULONG ResultLength)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("NtQueryValueKey");
        if (!function_address) return STATUS_ACCESS_VIOLATION;
        using Fn = NTSTATUS(*)(HANDLE, PUNICODE_STRING, KEY_VALUE_INFORMATION_CLASS, PVOID, ULONG, PULONG);
        return reinterpret_cast<Fn>(function_address)(KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, Length, ResultLength);
    }

    inline NTSTATUS NtQueryInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("NtQueryInformationFile");
        if (!function_address) return STATUS_ACCESS_VIOLATION;
        using Fn = NTSTATUS(*)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
        return reinterpret_cast<Fn>(function_address)(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);
    }

    inline NTSTATUS IoGetDeviceInterfaces(const GUID* InterfaceClassGuid, PDEVICE_OBJECT PhysicalDeviceObject, ULONG Flags, PZZWSTR* SymbolicLinkList)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("IoGetDeviceInterfaces");
        if (!function_address) return STATUS_ACCESS_VIOLATION;
        using Fn = NTSTATUS(*)(const GUID*, PDEVICE_OBJECT, ULONG, PZZWSTR*);
        return reinterpret_cast<Fn>(function_address)(InterfaceClassGuid, PhysicalDeviceObject, Flags, SymbolicLinkList);
    }

    inline HANDLE PsGetProcessInheritedFromUniqueProcessId(PEPROCESS Process)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("PsGetProcessInheritedFromUniqueProcessId");
        if (!function_address) return nullptr;
        using Fn = HANDLE(*)(PEPROCESS);
        return reinterpret_cast<Fn>(function_address)(Process);
    }

    inline NTSTATUS IoEnumerateDeviceObjectList(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT* DeviceObjectList, ULONG DeviceObjectListSize, PULONG ActualNumberofDeviceObjects)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("IoEnumerateDeviceObjectList");
        if (!function_address) return STATUS_ACCESS_VIOLATION;
        using Fn = NTSTATUS(*)(PDRIVER_OBJECT, PDEVICE_OBJECT*, ULONG, PULONG);
        return reinterpret_cast<Fn>(function_address)(DriverObject, DeviceObjectList, DeviceObjectListSize, ActualNumberofDeviceObjects);
    }

    inline PVOID PsGetThreadWin32Thread(PETHREAD Thread)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("PsGetThreadWin32Thread");
        if (!function_address) return nullptr;
        using Fn = PVOID(*)(PETHREAD);
        return reinterpret_cast<Fn>(function_address)(Thread);
    }

    inline NTSTATUS NtWriteFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("NtWriteFile");
        if (!function_address) return STATUS_ACCESS_VIOLATION;
        using Fn = NTSTATUS(*)(HANDLE, HANDLE, PIO_APC_ROUTINE, PVOID, PIO_STATUS_BLOCK, PVOID, ULONG, PLARGE_INTEGER, PULONG);
        return reinterpret_cast<Fn>(function_address)(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);
    }

    inline ULONG RtlWalkFrameChain(PVOID* Callers, ULONG Count, ULONG Flags)
    {
        static uintptr_t function_address = 0;
        if (!function_address) function_address = Resolver.GetSystemRoutine("RtlWalkFrameChain");
        if (!function_address) return 0;
        using Fn = ULONG(*)(PVOID*, ULONG, ULONG);
        return reinterpret_cast<Fn>(function_address)(Callers, Count, Flags);
    }


    inline void IofCompleteRequest(IRP* Irp, CHAR PriorityBoost)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IofCompleteRequest");

        if (!function_address)
            return;

        using Fn = VOID(*)(IRP*, CHAR);
        reinterpret_cast<Fn>(function_address)(Irp, PriorityBoost);
    }

    inline void RtlInitUnicodeString(PUNICODE_STRING Destination, PCWSTR Source)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("RtlInitUnicodeString");

        if (!function_address)
            return;

        using Fn = VOID(*)(PUNICODE_STRING, PCWSTR);
        reinterpret_cast<Fn>(function_address)(Destination, Source);
    }

    inline PVOID ExAllocatePool(POOL_TYPE PoolType, SIZE_T Size)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ExAllocatePool");

        if (!function_address)
            return nullptr;

        using Fn = PVOID(*)(POOL_TYPE, SIZE_T);
        return reinterpret_cast<Fn>(function_address)(PoolType, Size);
    }

    inline NTSTATUS IoDeleteDevice(PDEVICE_OBJECT DeviceObject)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoDeleteDevice");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PDEVICE_OBJECT);
        return reinterpret_cast<Fn>(function_address)(DeviceObject);
    }

    inline NTSTATUS IoDeleteSymbolicLink(PUNICODE_STRING SymbolicLink)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoDeleteSymbolicLink");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PUNICODE_STRING);
        return reinterpret_cast<Fn>(function_address)(SymbolicLink);
    }

    template<class... args_t>
    inline std::int8_t DbgPrintEx(const char* format, args_t... va_args) {
        static uintptr_t function_address = 0;
        if (!function_address) {
            function_address = Resolver.GetSystemRoutine("DbgPrintEx");
        }
        if (!function_address) return 0;

        using function_t = std::int32_t(std::uint32_t flag, std::uint32_t level,
            const char* format, args_t... va_args);
        return reinterpret_cast<function_t*>(function_address)(0, 0, format, va_args...) ==
            STATUS_SUCCESS;
    }
    inline NTSTATUS IoCreateDevice(
        PDRIVER_OBJECT DriverObject,
        ULONG DeviceExtensionSize,
        PUNICODE_STRING DeviceName,
        DEVICE_TYPE DeviceType,
        ULONG Characteristics,
        BOOLEAN Exclusive,
        PDEVICE_OBJECT* DeviceObject
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoCreateDevice");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PDRIVER_OBJECT, ULONG, PUNICODE_STRING, DEVICE_TYPE, ULONG, BOOLEAN, PDEVICE_OBJECT*);
        return reinterpret_cast<Fn>(function_address)(
            DriverObject,
            DeviceExtensionSize,
            DeviceName,
            DeviceType,
            Characteristics,
            Exclusive,
            DeviceObject
            );
    }
    inline NTSTATUS IoCreateDriver(
        PUNICODE_STRING DriverName,
        PDRIVER_INITIALIZE InitFunction
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoCreateDriver");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PUNICODE_STRING, PDRIVER_INITIALIZE);
        return reinterpret_cast<Fn>(function_address)(DriverName, InitFunction);
    }


    inline NTSTATUS IoCreateSymbolicLink(PUNICODE_STRING Symlink, PUNICODE_STRING Device)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoCreateSymbolicLink");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PUNICODE_STRING, PUNICODE_STRING);
        return reinterpret_cast<Fn>(function_address)(Symlink, Device);
    }

    inline void ObfDereferenceObject(PVOID Object)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ObfDereferenceObject");

        if (!function_address)
            return;

        using Fn = VOID(*)(PVOID);
        reinterpret_cast<Fn>(function_address)(Object);
    }

    inline NTSTATUS ObReferenceObjectByName(
        PUNICODE_STRING ObjectName,
        ULONG Attributes,
        PACCESS_STATE AccessState,
        ACCESS_MASK DesiredAccess,
        POBJECT_TYPE ObjectType,
        KPROCESSOR_MODE AccessMode,
        PVOID ParseContext,
        PVOID* Object
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ObReferenceObjectByName");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PUNICODE_STRING, ULONG, PACCESS_STATE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PVOID, PVOID*);
        return reinterpret_cast<Fn>(function_address)(
            ObjectName,
            Attributes,
            AccessState,
            DesiredAccess,
            ObjectType,
            AccessMode,
            ParseContext,
            Object
        );
    }

    inline NTSTATUS ObOpenObjectByPointer(
        PVOID Object,
        ULONG HandleAttributes,
        PACCESS_STATE PassedAccessState,
        ACCESS_MASK DesiredAccess,
        POBJECT_TYPE ObjectType,
        KPROCESSOR_MODE AccessMode,
        PHANDLE Handle
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ObOpenObjectByPointer");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PVOID, ULONG, PACCESS_STATE, ACCESS_MASK, POBJECT_TYPE, KPROCESSOR_MODE, PHANDLE);
        return reinterpret_cast<Fn>(function_address)(
            Object,
            HandleAttributes,
            PassedAccessState,
            DesiredAccess,
            ObjectType,
            AccessMode,
            Handle
        );
    }

    inline NTSTATUS PsLookupProcessByProcessId(
        HANDLE ProcessId,
        PEPROCESS* Process
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("PsLookupProcessByProcessId");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PEPROCESS*);
        return reinterpret_cast<Fn>(function_address)(ProcessId, Process);
    }

    inline PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process)
    {
        static uintptr_t function_address = 0;
        if (!function_address)
            function_address = Resolver.GetSystemRoutine("PsGetProcessSectionBaseAddress");

        if (!function_address)
            return nullptr;

        using function_t = PVOID(NTAPI*)(PEPROCESS);
        return reinterpret_cast<function_t>(function_address)(Process);
    }


    inline PPHYSICAL_MEMORY_RANGE MmGetPhysicalMemoryRanges()
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmGetPhysicalMemoryRanges");

        if (!function_address)
            return nullptr;

        using Fn = PPHYSICAL_MEMORY_RANGE(*)();
        return reinterpret_cast<Fn>(function_address)();
    }

    inline PVOID MmGetVirtualForPhysical(PHYSICAL_ADDRESS Physical)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmGetVirtualForPhysical");

        if (!function_address)
            return nullptr;

        using Fn = PVOID(*)(PHYSICAL_ADDRESS);
        return reinterpret_cast<Fn>(function_address)(Physical);
    }

    inline NTSTATUS MmCopyMemory(
        PVOID TargetAddress,
        MM_COPY_ADDRESS SourceAddress,
        SIZE_T NumberOfBytes,
        ULONG Flags,
        PSIZE_T BytesCopied
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmCopyMemory");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PVOID, MM_COPY_ADDRESS, SIZE_T, ULONG, PSIZE_T);
        return reinterpret_cast<Fn>(function_address)(
            TargetAddress,
            SourceAddress,
            NumberOfBytes,
            Flags,
            BytesCopied
            );
    }
    inline int RtlCompareUnicodeString(
        PUNICODE_STRING String1,
        PUNICODE_STRING String2,
        BOOLEAN CaseInsensitive
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("RtlCompareUnicodeString");

        if (!function_address)
            return 0;

        using Fn = LONG(*)(PUNICODE_STRING, PUNICODE_STRING, BOOLEAN);
        return reinterpret_cast<Fn>(function_address)(String1, String2, CaseInsensitive);
    }

    inline std::uintptr_t GetModuleBase(PCWSTR ModuleName)
    {
        UNICODE_STRING moduleNameString{};
        RtlInitUnicodeString(&moduleNameString, ModuleName);

        // Correct type: it is a KLDR entry, not a LIST_ENTRY.
        auto psLoadedModuleList = reinterpret_cast<_KLDR_DATA_TABLE_ENTRY*>(
            Resolver.GetSystemRoutine("PsLoadedModuleList")
            );

        if (!psLoadedModuleList)
            return 0;

        // List head is the LIST_ENTRY inside the first entry
        auto listHead = &psLoadedModuleList->InLoadOrderLinks;

        // Start with head->Flink
        for (PLIST_ENTRY link = listHead->Flink; link != listHead; link = link->Flink)
        {
            auto entry = CONTAINING_RECORD(link, _KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (RtlCompareUnicodeString(&entry->BaseDllName, &moduleNameString, TRUE) == 0)
            {
                return reinterpret_cast<std::uintptr_t>(entry->DllBase);
            }
        }

        return 0;
    }

    
    inline LIST_ENTRY* KeCapturePersistentThreadState() {
        static LIST_ENTRY* ps_active_process_head = nullptr;
        if (!ps_active_process_head) {
            static unsigned char* function_address = 0;
            if (!function_address) {
                function_address = (unsigned char*)Resolver.GetSystemRoutine("KeCapturePersistentThreadState");
                if (!function_address) return {};
            }

            while (function_address[0x0] != 0x20
                || function_address[0x1] != 0x48
                || function_address[0x2] != 0x8d)
                function_address++;

            ps_active_process_head = *reinterpret_cast<LIST_ENTRY**>
                (&function_address[0x8] + *reinterpret_cast<std::int32_t*>(&function_address[0x4]));
        }

        return ps_active_process_head;
    }

   inline std::uintptr_t PsInitialSystemProcess() {
        static uintptr_t function_address = 0;
        if (!function_address) {
            function_address = Resolver.GetSystemRoutine("PsInitialSystemProcess");
            if (!function_address) return {};
        }

        return *reinterpret_cast<std::uintptr_t*>(function_address);
    }

    inline bool MmIsAddressValid(void* VirtualAddress) {
        static uintptr_t function_address = 0;
        if (!function_address) {
            function_address = Resolver.GetSystemRoutine("MmIsAddressValid");
            if (!function_address) return {};
        }

        using function_t = bool(
            void* VirtualAddress
            );

        return reinterpret_cast<function_t*>(function_address)(VirtualAddress);
    }


    inline std::uint32_t PsGetProcessId(
        PEPROCESS* process
    ) {
        static uintptr_t function_address = 0;
        if (!function_address) {
            function_address = Resolver.GetSystemRoutine("PsGetProcessId");
            if (!function_address) return {};
        }

        using function_t = void* (
            PEPROCESS* Process
            );

        return reinterpret_cast<std::uint32_t>(
            reinterpret_cast<function_t*>(function_address)(process)
            );
    }

    inline NTSTATUS ZwQueryInformationProcess(
        HANDLE ProcessHandle,
        PROCESSINFOCLASS ProcessInformationClass,
        PVOID ProcessInformation,
        ULONG ProcessInformationLength,
        PULONG ReturnLength
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwQueryInformationProcess");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
        return reinterpret_cast<Fn>(function_address)(
            ProcessHandle,
            ProcessInformationClass,
            ProcessInformation,
            ProcessInformationLength,
            ReturnLength
        );
    }

    inline VOID KeStackAttachProcess(
        PEPROCESS Process,
        PRKAPC_STATE ApcState
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("KeStackAttachProcess");

        if (!function_address)
            return;

        using Fn = VOID(*)(PEPROCESS, PRKAPC_STATE);
        reinterpret_cast<Fn>(function_address)(Process, ApcState);
    }

    inline VOID KeUnstackDetachProcess(
        PRKAPC_STATE ApcState
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("KeUnstackDetachProcess");

        if (!function_address)
            return;

        using Fn = VOID(*)(PRKAPC_STATE);
        reinterpret_cast<Fn>(function_address)(ApcState);
    }

    inline KIRQL KeGetCurrentIrql()
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("KeGetCurrentIrql");

        if (!function_address)
            return PASSIVE_LEVEL;

        using Fn = KIRQL(*)();
        return reinterpret_cast<Fn>(function_address)();
    }

    inline VOID KeLowerIrql(KIRQL NewIrql)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("KeLowerIrql");

        if (!function_address)
            return;

        using Fn = VOID(*)(KIRQL);
        reinterpret_cast<Fn>(function_address)(NewIrql);
    }

    inline KIRQL KfRaiseIrql(KIRQL NewIrql)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("KfRaiseIrql");

        if (!function_address)
            return PASSIVE_LEVEL;

        using Fn = KIRQL(*)(KIRQL);
        return reinterpret_cast<Fn>(function_address)(NewIrql);
    }

    inline NTSTATUS ZwProtectVirtualMemory(
        HANDLE ProcessHandle,
        PVOID* BaseAddress,
        SIZE_T* NumberOfBytesToProtect,
        ULONG NewAccessProtection,
        PULONG OldAccessProtection
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwProtectVirtualMemory");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PVOID*, SIZE_T*, ULONG, PULONG);
        return reinterpret_cast<Fn>(function_address)(
            ProcessHandle,
            BaseAddress,
            NumberOfBytesToProtect,
            NewAccessProtection,
            OldAccessProtection
        );
    }

    inline NTSTATUS ZwFreeVirtualMemory(
        HANDLE ProcessHandle,
        PVOID* BaseAddress,
        PSIZE_T RegionSize,
        ULONG FreeType
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwFreeVirtualMemory");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PVOID*, PSIZE_T, ULONG);
        return reinterpret_cast<Fn>(function_address)(
            ProcessHandle,
            BaseAddress,
            RegionSize,
            FreeType
        );
    }

    inline NTSTATUS ZwAllocateVirtualMemory(
        HANDLE ProcessHandle,
        PVOID* BaseAddress,
        ULONG_PTR ZeroBits,
        PSIZE_T RegionSize,
        ULONG AllocationType,
        ULONG Protect
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwAllocateVirtualMemory");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
        return reinterpret_cast<Fn>(function_address)(
            ProcessHandle,
            BaseAddress,
            ZeroBits,
            RegionSize,
            AllocationType,
            Protect
        );
    }

    inline NTSTATUS RtlCreateUserThread(
        HANDLE ProcessHandle,
        PSECURITY_DESCRIPTOR SecurityDescriptor,
        BOOLEAN CreateSuspended,
        ULONG StackZeroBits,
        SIZE_T StackReserve,
        SIZE_T StackCommit,
        PVOID StartAddress,
        PVOID Parameter,
        PHANDLE ThreadHandle,
        PCLIENT_ID ClientId
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("RtlCreateUserThread");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE, PSECURITY_DESCRIPTOR, BOOLEAN, ULONG, SIZE_T, SIZE_T, PVOID, PVOID, PHANDLE, PCLIENT_ID);
        return reinterpret_cast<Fn>(function_address)(
            ProcessHandle,
            SecurityDescriptor,
            CreateSuspended,
            StackZeroBits,
            StackReserve,
            StackCommit,
            StartAddress,
            Parameter,
            ThreadHandle,
            ClientId
        );
    }

    inline NTSTATUS ZwClose(HANDLE Handle)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwClose");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(HANDLE);
        return reinterpret_cast<Fn>(function_address)(Handle);
    }
    inline PVOID ExAllocatePoolWithTag(POOL_TYPE PoolType, SIZE_T NumberOfBytes, ULONG Tag)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ExAllocatePoolWithTag");

        if (!function_address)
            return nullptr;

        using Fn = PVOID(*)(POOL_TYPE, SIZE_T, ULONG);
        return reinterpret_cast<Fn>(function_address)(PoolType, NumberOfBytes, Tag);
    }
    inline NTSTATUS ZwQuerySystemInformation(
        _In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,
        _Inout_ PVOID SystemInformation,
        _In_ ULONG SystemInformationLength,
        _Out_opt_ PULONG ReturnLength
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ZwQuerySystemInformation");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(NTAPI*)(
            SYSTEM_INFORMATION_CLASS,
            PVOID,
            ULONG,
            PULONG
            );

        return reinterpret_cast<Fn>(function_address)(
            SystemInformationClass,
            SystemInformation,
            SystemInformationLength,
            ReturnLength
            );
    }

    inline VOID ExFreePoolWithTag(
        _Pre_notnull_ __drv_freesMem(Mem) PVOID P,
        _In_ ULONG Tag
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("ExFreePoolWithTag");

        if (!function_address)
            return;

        using Fn = VOID(NTAPI*)(PVOID, ULONG);
        reinterpret_cast<Fn>(function_address)(P, Tag);
    }

    inline PMDL IoAllocateMdl(
        PVOID VirtualAddress,
        ULONG Length,
        BOOLEAN SecondaryBuffer,
        BOOLEAN ChargeQuota,
        PIRP Irp
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoAllocateMdl");

        if (!function_address)
            return nullptr;

        using Fn = PMDL(*)(PVOID, ULONG, BOOLEAN, BOOLEAN, PIRP);
        return reinterpret_cast<Fn>(function_address)(
            VirtualAddress,
            Length,
            SecondaryBuffer,
            ChargeQuota,
            Irp
            );
    }

    inline VOID IoFreeMdl(PMDL Mdl)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("IoFreeMdl");

        if (!function_address)
            return;

        using Fn = VOID(*)(PMDL);
        reinterpret_cast<Fn>(function_address)(Mdl);
    }

    inline VOID MmProbeAndLockPages(
        PMDL MemoryDescriptorList,
        KPROCESSOR_MODE AccessMode,
        LOCK_OPERATION Operation
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmProbeAndLockPages");

        if (!function_address)
            return;

        using Fn = VOID(*)(PMDL, KPROCESSOR_MODE, LOCK_OPERATION);
        reinterpret_cast<Fn>(function_address)(
            MemoryDescriptorList,
            AccessMode,
            Operation
            );
    }

    inline PVOID MmMapLockedPagesSpecifyCache(
        PMDL MemoryDescriptorList,
        KPROCESSOR_MODE AccessMode,
        MEMORY_CACHING_TYPE CacheType,
        PVOID RequestedAddress,
        ULONG BugCheckOnFailure,
        ULONG Priority
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmMapLockedPagesSpecifyCache");

        if (!function_address)
            return nullptr;

        using Fn = PVOID(*)(PMDL, KPROCESSOR_MODE, MEMORY_CACHING_TYPE, PVOID, ULONG, ULONG);
        return reinterpret_cast<Fn>(function_address)(
            MemoryDescriptorList,
            AccessMode,
            CacheType,
            RequestedAddress,
            BugCheckOnFailure,
            Priority
            );
    }

    inline NTSTATUS MmProtectMdlSystemAddress(
        PMDL MemoryDescriptorList,
        ULONG NewProtect
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmProtectMdlSystemAddress");

        if (!function_address)
            return STATUS_ACCESS_VIOLATION;

        using Fn = NTSTATUS(*)(PMDL, ULONG);
        return reinterpret_cast<Fn>(function_address)(
            MemoryDescriptorList,
            NewProtect
            );
    }

    inline VOID MmUnmapLockedPages(
        PVOID BaseAddress,
        PMDL MemoryDescriptorList
    )
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmUnmapLockedPages");

        if (!function_address)
            return;

        using Fn = VOID(*)(PVOID, PMDL);
        reinterpret_cast<Fn>(function_address)(BaseAddress, MemoryDescriptorList);
    }

    inline VOID MmUnlockPages(PMDL MemoryDescriptorList)
    {
        static uintptr_t function_address = 0;

        if (!function_address)
            function_address = Resolver.GetSystemRoutine("MmUnlockPages");

        if (!function_address)
            return;

        using Fn = VOID(*)(PMDL);
        reinterpret_cast<Fn>(function_address)(MemoryDescriptorList);
    }

};

#define ExFreePooll(a) NT.ExFreePoolWithTag (a,0)
