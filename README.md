# WinKnlHook

A 64-bit Windows 10/11 kernel-mode driver operating at Ring 0, designed for low-level memory operations, physical memory translation, signature scanning, and kernel-space mouse input simulation.

> [!WARNING]
> This driver contains critical stability bugs and skeleton modules. It is intended for educational and security research purposes only.

---

## Features
* **Zero-Attachment RWX Engine**: Translates virtual addresses to physical addresses via a custom page-table walker (`PML4 -> PDPT -> PD -> PT`) and performs physical memory read/write operations without attaching to target processes.
* **Directory Base (CR3) Decryption**: Dynamically locates the PFN database (`MmPfnDatabase`) using signature scanning to resolve target process directory bases, bypassing CR3 scrambling/protection.
* **Dynamic Export Resolver**: Resolves `ntoskrnl.exe` routines dynamically by scanning the PE export directory and walking the x64 KPCR stack (`gs:[18h]`), eliminating static imports.
* **Kernel Mouse Simulation**: Walks the `\Driver\MouClass` and `\Driver\MouHID` device stacks to directly invoke `MouseClassServiceCallback` at `DISPATCH_LEVEL` via an assembly wrapper.
* **Signature Scanner**: Securely scans target process address spaces with access verification (`MmIsAddressValid`) and structured exception handling (`__try/__except`).
* **HWID Spoofer**: Employs registry callbacks (`CmRegisterCallback`) and IRP dispatch hooks to spoof disk serials, SMBIOS, UUID, and MAC addresses.

---

## Quick Setup

### 1. Compile
1. Open [RWH.sln](RWH.sln) in Visual Studio as **Administrator**.
2. Build the solution under **Release | x64** using the Windows Driver Kit (WDK) to generate `RWH.sys`.

### 2. Load (Manual Mapping)
Because the driver is designed with a parameterless entry point (`NTSTATUS DriverEntry()`) and uses dynamic kernel base resolution, it must be loaded using a **manual mapper** (such as [kdmapper](https://github.com/TheCruZ/kdmapper)) rather than standard service registration:

1. Download or compile a reliable manual mapper (e.g., `kdmapper.exe`).
2. Disable Windows Defender or any anti-virus software (as they block vulnerable driver exploits used by mappers).
3. Open an elevated Command Prompt (`cmd` as Administrator) and run:
   ```cmd
   kdmapper.exe RWH.sys
   ```
   *Note: Manual mappers exploit a vulnerable signed driver to map your unsigned driver into kernel space and execute its entry point, bypassing Driver Signature Enforcement (DSE) without needing to enable Test Signing mode.*

---

## IOCTL Quick Reference

Device Link: `\\.\WinKnlHook`

| Request | IOCTL | Buffer Structure | Description |
| :--- | :--- | :--- | :--- |
| **Get Base Address** | `0x2A2` | `base_new { INT32 process_id; ULONGLONG BaseAddress; }` | Gets process section base address |
| **Decrypt CR3** | `0x2A3` | `dtb { uint64_t BaseAddress; uint64_t outaddress; }` | Decrypts process Directory Base |
| **Read Memory** | `0x2A4` | `read { ULONGLONG address; ULONGLONG buffer; ULONGLONG size; }` | Reads process memory physically |
| **Write Memory** | `0x2A5` | `write { ULONGLONG address; ULONGLONG buffer; ULONGLONG size; }` | Writes process memory physically |
| **Pattern Scan** | `0x2A6` | `pattern_scan { ... }` *(Check IoCodes.h)* | Runs secure signature scan |
| **Move Mouse** | `0x3A1` | `movemouse { long x; long y; unsigned short button_flags; }` | Simulates direct mouse movement |

---

## Known Stability Issues & Critical Bugs

If deploying this driver, be aware of these severe kernel-level flaws:

1. **[CRITICAL] Mouse Stack OOB Read** ([MouseMove.cpp:L60-71](Modules/MouseMove/MouseMove.cpp#L60-L71)): 
   The device extension size calculation divides by `4` instead of `8`. This causes the callback traversal loop to scan 100% past the buffer bounds, leading to random page faults and **BSODs**.
2. **[CRITICAL] EPROCESS Reference Leak** ([Base.cpp:L34-41](Modules/Memory/BaseAddress/Base.cpp#L34-L41)): 
   If process base retrieval fails, the driver exits without calling `ObfDereferenceObject`. This permanently leaks the process object in kernel memory.
3. **[HIGH] PML4 Cache Race Condition** ([RWX.cpp:L56-63](Modules/MemoryAccess/RWX/RWX.cpp#L56-L63)): 
   The global translation cache `cached_pml4e` has no thread synchronization. Concurrent user-space IOCTL reads/writes will corrupt the cache, causing wrong memory mappings and crashes.
4. **[MEDIUM] Dead Code & Skeletons**: 
   The driver hiding routine `DestroyDriverInformation` is never called. Additionally, the entire Easy Anti-Cheat (EAC) bypass module (`Modules/EAC/`) consists of empty skeleton functions that do nothing.

---

## Disclaimer
This software is provided for educational, security research, and testing purposes only. Portions of this codebase were modified, updated, or developed with the assistance of Artificial Intelligence (AI) models. Operating in kernel mode carries inherent risks, including system instability and security vulnerabilities. The authors and contributors assume no liability for misuse, system damage, or issues arising from the deployment of this driver in production or untrusted environments.
