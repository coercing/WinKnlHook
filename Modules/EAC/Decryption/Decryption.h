#pragma once
#include "../../../Includes.h"


typedef struct _call_decrypt_imports {
    uint64_t EncryptedValueAddr;      
    uint64_t Result;                 
} call_decrypt_imports, * pcall_decrypt_imports;



using DecryptImportFn = UINT64(__fastcall*)(UINT64* EncryptedPointer);
using MmCopyMemoryFn = NTSTATUS(__fastcall*)(
    PVOID TargetAddress,
    MM_COPY_ADDRESS SourceAddress,
    SIZE_T NumberOfBytes,
    ULONG Flags,
    PSIZE_T NumberOfBytesTransferred
    );
using PsGetProcessSectionBaseAddressFn = PVOID(__fastcall*)(PEPROCESS Process);
using PsInitialSystemProcessPtr = PEPROCESS*;
using MmIsAddressValidFn = BOOLEAN(__fastcall*)(PVOID VirtualAddress);
using PsGetProcessIdFn = HANDLE(__fastcall*)(PEPROCESS Process);
namespace Decryption
{
    UINT64* ExtractEncryptedPointer(uintptr_t callSiteAddress);
    UINT64 CallDecryptImport(UINT64* encryptedPtr);
    UINT64 ApplyFinalDecryption(UINT64 decryptedValue, UINT8 rotationCount, UINT64 xorKey);
}

namespace DecryptionFunctions
{
    extern MmCopyMemoryFn MmCopyMemory_Decrypted;
    extern PsGetProcessSectionBaseAddressFn PsGetProcessSectionBaseAddress_Decrypted;
    extern PsInitialSystemProcessPtr PsInitialSystemProcess_Decrypted;
    extern MmIsAddressValidFn MmIsAddressValid_Decrypted;
    extern PsGetProcessIdFn PsGetProcessId_Decrypted;
    
    void DecryptMmCopyMemory(uintptr_t callSiteAddress);
    void DecryptPsGetProcessSectionBaseAddress(uintptr_t callSiteAddress);
    void DecryptPsInitialSystemProcess(uintptr_t callSiteAddress);
    void DecryptMmIsAddressValid(uintptr_t callSiteAddress);
    void DecryptPsGetProcessId(uintptr_t callSiteAddress);
}
