#pragma once

#include "../../Includes.h"

namespace DriverInit
{
	NTSTATUS UnsupportedDispatch(PDEVICE_OBJECT DriverObject, PIRP irp);
	NTSTATUS DispatchHandler(PDEVICE_OBJECT DriverObject, PIRP irp);
	void DriverUnloading(PDRIVER_OBJECT DriverObject);
	BOOLEAN DestroyDriverInformation(PDRIVER_OBJECT pDriverObject, WCHAR* DriverName, ULONG64 DriverAddress, BOOLEAN bByName);
}


//NTSTATUS DriverInitialize(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
//{
//	UNREFERENCED_PARAMETER(DriverObject);
//
//	NTSTATUS StatusResult = { };
//	PDEVICE_OBJECT deviceObj = { };
//	printf("[+] ------------------------------------------------\n");
//	printf("[+] Started Driver Initialized.\n");
//	NT.RtlInitUnicodeString(&DeviceN, DEVICE_MODULE);
//	NT.RtlInitUnicodeString(&DosL, DEVICE_DOS);
//
//	printf("[+] DeviceN -> %wZ\n", &DeviceN);
//	printf("[+] DosL -> %wZ\n", &DosL);
//
//	StatusResult = (NT.IoCreateDevice)(DriverObject, 0, &DeviceN, FILE_DEVICE_NETWORK, FILE_DEVICE_SECURE_OPEN, FALSE, &deviceObj);
//	if (!NT_SUCCESS(StatusResult))
//	{
//		printf("[-] Couldn't Create Device.\n");
//		return StatusResult;
//	}
//	printf("[+] Created Device.\n");
//	StatusResult = (NT.IoCreateSymbolicLink)(&DosL, &DeviceN);
//	if (!NT_SUCCESS(StatusResult))
//	{
//		printf("[-] Couldn't Create SymbolicLink.\n");
//		return StatusResult;
//	}
//	printf("[+] Created SymbolicLink.\n");
//
//	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
//		DriverObject->MajorFunction[i] = &DriverInit::UnsupportedDispatch;
//
//	deviceObj->Flags |= DO_BUFFERED_IO;
//	DriverObject->MajorFunction[IRP_MJ_CREATE] = &DriverInit::DispatchHandler;
//	DriverObject->MajorFunction[IRP_MJ_CLOSE] = &DriverInit::DispatchHandler;
//	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &IoControl::Dispatch;
//	DriverObject->DriverUnload = &DriverInit::DriverUnloading;
//	deviceObj->Flags &= ~DO_DEVICE_INITIALIZING;
//	//
//	//DriverInit::DestroyDriverInformation(DriverObject, NULL, (ULONG64)DriverInit::DriverUnloading, FALSE);
// //   {
// //       PKLDR_DATA_TABLE_ENTRY pSelfEntry = nullptr;
// //       auto pNext = PsLoadedModuleList->Flink;
// //       if (pNext != NULL)
// //       {
// //           while (pNext != PsLoadedModuleList)
// //           {
// //               auto pEntry = CONTAINING_RECORD(pNext, KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
//
// //               auto pBase = pEntry->DllBase;
// //               if (DriverObject->DriverStart == pBase)
// //               {
// //                   pSelfEntry = pEntry;
// //                   printf("[+] find world:%p, DllBase=%p\n", pSelfEntry, pBase);
// //                   break;
// //               }
//
// //               pNext = pNext->Flink;
// //           }
// //       }
//
// //       // hide world
// //       if (pSelfEntry)
// //       {
// //           KIRQL kIrql = KeRaiseIrqlToDpcLevel();
// //           auto pPrevEntry = (PKLDR_DATA_TABLE_ENTRY)pSelfEntry->InLoadOrderLinks.Blink;
// //           auto pNextEntry = (PKLDR_DATA_TABLE_ENTRY)pSelfEntry->InLoadOrderLinks.Flink;
//
// //           if (pPrevEntry)
// //           {
// //               pPrevEntry->InLoadOrderLinks.Flink = pSelfEntry->InLoadOrderLinks.Flink;
// //           }
//
// //           if (pNextEntry)
// //           {
// //               pNextEntry->InLoadOrderLinks.Blink = pSelfEntry->InLoadOrderLinks.Blink;
// //           }
//
// //           pSelfEntry->InLoadOrderLinks.Flink = (PLIST_ENTRY)pSelfEntry;
// //           pSelfEntry->InLoadOrderLinks.Blink = (PLIST_ENTRY)pSelfEntry;
//
// //           KeLowerIrql(kIrql);
//
// //           printf("[+] swapped loaded module\n");
// //       }
// //   }
//
//
//	
//	printf("[+] Welcome To Kernel H.\n");
//	printf("[+] ------------------------------------------------\n");
//	return StatusResult;
//}
//
//
//NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
//	UNREFERENCED_PARAMETER(DriverObject);
//	UNREFERENCED_PARAMETER(RegistryPath);
//	printf("[+] DriverEntry Just hit.\n");
//	if (!Resolver.SetupASM())
//	{
//		printf("[-] Couldn't setup ASM.\n");
//		return STATUS_UNSUCCESSFUL;
//	}
//	printf("[+] ASM successfully setup.\n");
//
//	mousemove::InitializeMouseControl();
//
//
//	return NT.IoCreateDriver(NULL, &DriverInitialize);
//}




