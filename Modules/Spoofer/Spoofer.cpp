#include "Spoofer.h"
#include "../../Core/Utilities/Randomizer.h"
#include <ntddstor.h>
#include <mountdev.h>
#include <ntdddisk.h>
#include <ntddscsi.h>
#include <ntddvol.h>
#include <wdm.h>
#include <ntifs.h>

// Fallback for Registry
#ifndef PREG_QUERY_VALUE_KEY
typedef struct _REG_QUERY_VALUE_KEY {
    PUNICODE_STRING ValueName;
    KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass;
    PVOID KeyValueInformation;
    ULONG Length;
    PULONG ResultLength;
} REG_QUERY_VALUE_KEY, * PREG_QUERY_VALUE_KEY;
#endif

namespace Spoofer
{
    //bool g_Initialized = false;
    ULONG64 g_CurrentSeed = 0;
    LARGE_INTEGER g_RegCookie = { 0 };

    char g_DiskSerial[128] = { 0 };
    char g_SMBIOSSerial[64] = { 0 };
    char g_UUID[64] = { 0 };
    unsigned char g_MAC[6] = { 0 };

    PDRIVER_DISPATCH OriginalStorageDispatch = nullptr;
    PDRIVER_DISPATCH OriginalWmiDispatch = nullptr;
    PDEVICE_OBJECT g_WmiDeviceObject = nullptr;

    void InitializeSeed()
    {
        LARGE_INTEGER tick;
        KeQueryTickCount(&tick);
        ULONG64 seed = tick.QuadPart ^ __rdtsc() ^ (ULONG64)PsGetCurrentProcessId() ^ (ULONG64)&InitializeSeed ^ 0xF15E1337DEADULL;
        g_CurrentSeed = seed ^ (seed >> 32) ^ (seed << 17);
    }

    inline char SeededRandomChar(ULONG64& seed)
    {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[seed % 36];
    }

    inline char SeededRandomHex(ULONG64& seed)
    {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return "0123456789ABCDEF"[seed % 16];
    }

    void GenerateTempSpoofData()
    {
        InitializeSeed();
        ULONG64 seed = g_CurrentSeed;

        // Disk Serial
        const char* prefixes[] = { "WD", "ST", "SN", "HGST", "VMware" };
        strcpy_s(g_DiskSerial, prefixes[seed % 5]);
        size_t len = strlen(g_DiskSerial);
        for (int i = 0; i < 20; i++) {
            g_DiskSerial[len++] = SeededRandomChar(seed);
            if (i == 5 || i == 11) g_DiskSerial[len++] = '_';
        }
        g_DiskSerial[len] = '\0';

        // SMBIOS 
        strcpy_s(g_SMBIOSSerial, "PF");
        for (int i = 0; i < 8; i++) g_SMBIOSSerial[2 + i] = SeededRandomChar(seed);
        g_SMBIOSSerial[10] = '\0';

        // UUID for Win32_ComputerSystemProduct
        char* u = g_UUID;
        for (int i = 0; i < 8; i++) *u++ = SeededRandomHex(seed);
        *u++ = '-'; for (int i = 0; i < 4; i++) *u++ = SeededRandomHex(seed);
        *u++ = '-'; *u++ = '1'; for (int i = 0; i < 3; i++) *u++ = SeededRandomHex(seed);
        *u++ = '-'; *u++ = '8'; for (int i = 0; i < 3; i++) *u++ = SeededRandomHex(seed);
        *u++ = '-'; for (int i = 0; i < 12; i++) *u++ = SeededRandomHex(seed);
        *u = '\0';

        // MAC
        g_MAC[0] = 0x02;
        g_MAC[1] = 0x1A; g_MAC[2] = 0x2B;
        for (int i = 3; i < 6; i++) {
            seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
            g_MAC[i] = (UCHAR)seed;
        }
    }

    NTSTATUS NTAPI RegistryCallback(PVOID CallbackContext, PVOID Argument1, PVOID Argument2)
    {
        UNREFERENCED_PARAMETER(CallbackContext);
        if ((ULONG_PTR)Argument1 != RegNtPreQueryValueKey) return STATUS_SUCCESS;

        __try
        {
            PREG_QUERY_VALUE_KEY pre = (PREG_QUERY_VALUE_KEY)Argument2;
            if (!pre || !pre->ValueName || !pre->ValueName->Buffer || !pre->KeyValueInformation)
                return STATUS_SUCCESS;

            WCHAR* name = pre->ValueName->Buffer;

            if (wcsstr(name, L"Serial") || wcsstr(name, L"BIOS") || wcsstr(name, L"BaseBoard"))
            {
                ULONG len = min(pre->Length, (ULONG)strlen(g_SMBIOSSerial) + 1);
                RtlCopyMemory(pre->KeyValueInformation, g_SMBIOSSerial, len);
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {}
        return STATUS_SUCCESS;
    }
    
    // current SMBIOS is not working. icl this spoofer is pretty ass but i mean, being high and using AI can only get u so far.
    void PatchSMBIOSBuffer(PVOID buffer, ULONG size)
    {
        if (!buffer || size < 100) return;
        __try
        {
            char* buf = (char*)buffer;
            const char* targets[] = { "SerialNumber", "SMBIOSBIOSVersion", "BaseBoardSerialNumber" };

            for (ULONG i = 0; i < size - 64; ++i)
            {
                for (auto t : targets)
                {
                    if (_strnicmp(buf + i, t, strlen(t)) == 0)
                    {
                        char* val = buf + i + strlen(t) + 8;
                        if ((ULONG_PTR)(val - buf) + 32 < size)
                            strcpy_s(val, 32, g_SMBIOSSerial);
                    }
                }
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {}
    }

    NTSTATUS HookedWmiDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
    {
        if (Irp->AssociatedIrp.SystemBuffer && Irp->IoStatus.Information > 0)
            PatchSMBIOSBuffer(Irp->AssociatedIrp.SystemBuffer, (ULONG)Irp->IoStatus.Information);

        if (OriginalWmiDispatch)
            return OriginalWmiDispatch(DeviceObject, Irp);

        return Irp->IoStatus.Status;
    }

    NTSTATUS DiskControlCompletion(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context)
    {
        UNREFERENCED_PARAMETER(DeviceObject);
        UNREFERENCED_PARAMETER(Context);

        if (Irp->PendingReturned) IoMarkIrpPending(Irp);
        if (!NT_SUCCESS(Irp->IoStatus.Status)) return Irp->IoStatus.Status;

        PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
        if (stack->MajorFunction == IRP_MJ_DEVICE_CONTROL)
        {
            if (stack->Parameters.DeviceIoControl.IoControlCode == IOCTL_STORAGE_QUERY_PROPERTY)
            {
                PSTORAGE_PROPERTY_QUERY q = (PSTORAGE_PROPERTY_QUERY)Irp->AssociatedIrp.SystemBuffer;
                if (q && q->PropertyId == StorageDeviceProperty)
                {
                    PSTORAGE_DEVICE_DESCRIPTOR d = (PSTORAGE_DEVICE_DESCRIPTOR)Irp->AssociatedIrp.SystemBuffer;
                    if (d && d->SerialNumberOffset)
                    {
                        char* s = (char*)d + d->SerialNumberOffset;
                        strcpy_s(s, 128 - ((PUCHAR)s - (PUCHAR)d), g_DiskSerial);
                    }
                }
            }
        }
        return Irp->IoStatus.Status;
    }

    NTSTATUS HookedDiskDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
    {
        PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
        if (stack->MajorFunction == IRP_MJ_DEVICE_CONTROL)
        {
            IoCopyCurrentIrpStackLocationToNext(Irp);
            IoSetCompletionRoutine(Irp, DiskControlCompletion, nullptr, TRUE, TRUE, TRUE);
        }
        return OriginalStorageDispatch(DeviceObject, Irp);
    }

    NTSTATUS Initialize()
    {
        if (g_Initialized) return STATUS_SUCCESS;

        GenerateTempSpoofData();

        CmRegisterCallback(RegistryCallback, nullptr, &g_RegCookie);

        UNICODE_STRING wmiName = RTL_CONSTANT_STRING(L"\\Device\\WMIDataDevice");
        PFILE_OBJECT wmiFile = nullptr;
        if (NT_SUCCESS(IoGetDeviceObjectPointer(&wmiName, FILE_READ_DATA, &wmiFile, &g_WmiDeviceObject)) && g_WmiDeviceObject)
        {
            OriginalWmiDispatch = g_WmiDeviceObject->DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL];
            if (OriginalWmiDispatch)
                g_WmiDeviceObject->DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = HookedWmiDispatch;
        }

        const wchar_t* drv[] = { L"\\Driver\\Disk", L"\\Driver\\storahci", L"\\Driver\\stornvme" };
        for (int i = 0; i < 3; i++)
        {
            UNICODE_STRING name;
            RtlInitUnicodeString(&name, drv[i]);
            PFILE_OBJECT f = nullptr;
            PDEVICE_OBJECT d = nullptr;
            if (NT_SUCCESS(IoGetDeviceObjectPointer(&name, FILE_READ_DATA, &f, &d)) && d && d->DriverObject)
            {
                OriginalStorageDispatch = d->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL];
                if (OriginalStorageDispatch)
                {
                    d->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = HookedDiskDispatch;
                    break;
                }
            }
        }

        DbgPrint("LOADED | Disk: %s | SMBIOS: %s | UUID: %s\n", g_DiskSerial, g_SMBIOSSerial, g_UUID);

        g_Initialized = true;
        return STATUS_SUCCESS;
    }

    void Cleanup()
    {
        if (g_RegCookie.QuadPart) CmUnRegisterCallback(g_RegCookie);
        if (g_WmiDeviceObject && OriginalWmiDispatch)
            g_WmiDeviceObject->DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = OriginalWmiDispatch;

        g_Initialized = false;
    }
}