#include "Driverinit.h"

namespace DriverInit
{
	NTSTATUS UnsupportedDispatch(PDEVICE_OBJECT DriverObject, PIRP irp)
	{
		UNREFERENCED_PARAMETER(DriverObject);
		irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
		(NT.IofCompleteRequest)(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}
	NTSTATUS DispatchHandler(PDEVICE_OBJECT DriverObject, PIRP irp)
	{
		UNREFERENCED_PARAMETER(DriverObject);
		PIO_STACK_LOCATION stackLocation = IoGetCurrentIrpStackLocation(irp);
		switch (stackLocation->MajorFunction) {
		case IRP_MJ_CREATE:
			break;
		case IRP_MJ_CLOSE:
			break;
		default:
			break;
		}
		(NT.IofCompleteRequest)(irp, IO_NO_INCREMENT);
		return irp->IoStatus.Status;
	}
	void DriverUnloading(PDRIVER_OBJECT DriverObject)
	{
		NTSTATUS unlinkStatus = { 0 };
		unlinkStatus = (NT.IoDeleteSymbolicLink)(&DosL);
		if (!NT_SUCCESS(unlinkStatus))
			return;
		(NT.IoDeleteDevice)(DriverObject->DeviceObject);
	}



	BOOLEAN DestroyDriverInformation(PDRIVER_OBJECT pDriverObject, WCHAR* DriverName, ULONG64 DriverAddress, BOOLEAN bByName)
	{
		printf("[+] DestroyDriverInformation called\n");
		printf("[+] pDriverObject = %p\n", pDriverObject);
		printf("[+] DriverName = %ws\n", DriverName ? DriverName : L"NULL");
		printf("[+] DriverAddress = 0x%llx\n", DriverAddress);
		printf("[+] bByName = %d\n", bByName);

		ULONG Count = 0;

		PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
		printf("[+] Initial entry (DriverSection) = %p\n", entry);

		PLDR_DATA_TABLE_ENTRY firstentry = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Blink;
		printf("[+] First entry (Blink) = %p\n", firstentry);

		__try {
			if (!bByName)
			{
				printf("[+] Searching by address in first entry\n");
				printf("[+] firstentry->DllBase = %p\n", firstentry->DllBase);
				printf("[+] firstentry->SizeOfImage = 0x%x\n", firstentry->SizeOfImage);

				if (DriverAddress >= (ULONG64)firstentry->DllBase && DriverAddress < (ULONG64)firstentry->DllBase + firstentry->SizeOfImage)
				{
					printf("[+] Found driver by address in first entry!\n");
					printf("[+] Nuking BaseDllName and FullDllName\n");
					firstentry->BaseDllName.Length = 0;
					firstentry->BaseDllName.MaximumLength = 0;
					firstentry->FullDllName.Length = 0;
					firstentry->FullDllName.MaximumLength = 0;
					printf("[+] Successfully nuked driver info in first entry!\n");
				}
				else
				{
					printf("[-] Driver address not found in first entry\n");
				}
			}
			else
			{
				printf("[+] Searching by name in first entry\n");
				printf("[+] firstentry->BaseDllName.Buffer = %ws\n", firstentry->BaseDllName.Buffer);

				if (wcsstr(firstentry->BaseDllName.Buffer, DriverName))
				{
					printf("[+] Found driver by name in first entry!\n");
					printf("[+] Nuking BaseDllName and FullDllName\n");
					firstentry->FullDllName.Length = 0;
					firstentry->FullDllName.MaximumLength = 0;
					firstentry->BaseDllName.Length = 0;
					firstentry->BaseDllName.MaximumLength = 0;
					printf("[+] Successfully nuked driver info by name!\n");

					return TRUE;
				}
				else
				{
					printf("[-] Driver name not found in first entry\n");
				}
			}
		}
		__except (1)
		{
			printf("[-] Exception occurred while processing first entry!\n");
		}

		Count++;
		printf("[+] Starting iteration through module list (Count = %d)\n", Count);

		entry = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
		firstentry = entry;
		printf("[+] Initialized entry = %p, firstentry = %p\n", entry, firstentry);

		while ((PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstentry)
		{
			printf("[+] Processing entry %d: %p\n", Count, entry);
			printf("[+] entry->EntryPoint = %p\n", entry->EntryPoint);
			printf("[+] MmUserProbeAddress = %p\n", (PVOID)MmUserProbeAddress);

			if ((ULONG_PTR)entry->EntryPoint > MmUserProbeAddress)
			{
				printf("[+] Entry is in kernel space, processing...\n");
				__try {
					if (!bByName)
					{
						printf("[+] Searching by address\n");
						printf("[+] entry->DllBase = %p\n", entry->DllBase);
						printf("[+] entry->SizeOfImage = 0x%x\n", entry->SizeOfImage);

						if (DriverAddress >= (ULONG64)entry->DllBase && DriverAddress < (ULONG64)entry->DllBase + entry->SizeOfImage)
						{
							printf("[+] Found driver by address!\n");
							printf("[+] Nuking BaseDllName\n");
							entry->BaseDllName.Length = 0;
							entry->BaseDllName.MaximumLength = 0;
							printf("[+] Successfully nuked driver info by address!\n");
							return TRUE;
						}
						else
						{
							printf("[-] Address not in range for this entry\n");
						}
					}
					else
					{
						printf("[+] Searching by name\n");
						printf("[+] entry->BaseDllName.Buffer = %ws\n", entry->BaseDllName.Buffer);

						if (wcsstr(entry->BaseDllName.Buffer, DriverName))
						{
							printf("[+] Found driver by name!\n");
							printf("[+] Nuking BaseDllName\n");
							entry->BaseDllName.Length = 0;
							entry->BaseDllName.MaximumLength = 0;
							printf("[+] Successfully nuked driver info by name!\n");

							return TRUE;
						}
						else
						{
							printf("[-] Name not found for this entry\n");
						}
					}
				}
				__except (1)
				{
					printf("[-] Exception occurred while processing entry %d!\n", Count);
				}

				Count++;
			}
			else
			{
				printf("[-] Entry is in user space, skipping\n");
			}

			entry = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
			printf("[+] Moving to next entry: %p\n", entry);
		}

		printf("[-] Driver not found after checking %d entries\n", Count);
		return FALSE;
	}
}