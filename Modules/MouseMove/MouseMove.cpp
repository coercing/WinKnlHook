#include "MouseMove.h"
#define KeRaiseIrql(a,b) *(b) = (NT.KfRaiseIrql)(a)
//
//MOUSE_OBJECT gMouseObject;
//
//extern "C" {
//	ULONG64 _KeAcquireSpinLockAtDpcLevel = 0;
//	ULONG64 _KeReleaseSpinLockFromDpcLevel = 0;
//	ULONG64 _IofCompleteRequest = 0;
//	ULONG64 _IoReleaseRemoveLockEx = 0;
//}

namespace mousemove
{

	VOID InitializeMouseControl(VOID)
	{
		_KeAcquireSpinLockAtDpcLevel = (ULONG64)(KeAcquireSpinLockAtDpcLevel);
		_KeReleaseSpinLockFromDpcLevel = (ULONG64)(KeReleaseSpinLockFromDpcLevel);
		_IofCompleteRequest = (ULONG64)(IofCompleteRequest);
		_IoReleaseRemoveLockEx = (ULONG64)(IoReleaseRemoveLockEx);
	}
	BOOLEAN OpenMouse(VOID)
	{
		if (gMouseObject.UseMouse == 0) {
			UNICODE_STRING class_string;
			(NT.RtlInitUnicodeString)(&class_string, E(L"\\Driver\\MouClass"));
			
			PDRIVER_OBJECT ClassDriverObject = NULL;
			NTSTATUS status = (NT.ObReferenceObjectByName)(
				&class_string, OBJ_CASE_INSENSITIVE, NULL, 0, *(POBJECT_TYPE*)(IoDriverObjectType), KernelMode, NULL, (PVOID*)&ClassDriverObject
				);
			if (!NT_SUCCESS(status)) {
				gMouseObject.UseMouse = 0;
				return FALSE;
			}

			UNICODE_STRING hid_string;
			(NT.RtlInitUnicodeString)(&hid_string, E(L"\\Driver\\MouHID"));
			PDRIVER_OBJECT HidDriverObject = NULL;
			status = (NT.ObReferenceObjectByName)(
				&hid_string, OBJ_CASE_INSENSITIVE, NULL, 0, *(POBJECT_TYPE*)(IoDriverObjectType), KernelMode, NULL, (PVOID*)&HidDriverObject
				);

			if (!NT_SUCCESS(status)) {
				(NT.ObfDereferenceObject)(ClassDriverObject);
				gMouseObject.UseMouse = 0;
				return FALSE;
			}

			PDEVICE_OBJECT HidDeviceObject = HidDriverObject->DeviceObject;
			while (HidDeviceObject && !gMouseObject.ServiceCallback) {
				PDEVICE_OBJECT ClassDeviceObject = ClassDriverObject->DeviceObject;

				while (ClassDeviceObject && !gMouseObject.ServiceCallback) {
					if (!ClassDeviceObject->NextDevice && !gMouseObject.DeviceObject) {
						gMouseObject.DeviceObject = ClassDeviceObject;
					}

					PULONG64 device_extension = (PULONG64)HidDeviceObject->DeviceExtension;
					ULONG64 device_ext_size =
						((ULONG64)HidDeviceObject->DeviceObjectExtension - (ULONG64)HidDeviceObject->DeviceExtension) / 4;

					for (ULONG64 i = 0; i < device_ext_size; i++) {
						if (device_extension[i] == (ULONG64)ClassDeviceObject &&
							device_extension[i + 1] > (ULONG64)ClassDriverObject) {

							gMouseObject.ServiceCallback = (MouseClassServiceCallbackFn)(device_extension[i + 1]);
							break;
						}
					}

					ClassDeviceObject = ClassDeviceObject->NextDevice;
				}

				HidDeviceObject = HidDeviceObject->AttachedDevice;
			}

			if (!gMouseObject.DeviceObject) {
				PDEVICE_OBJECT TargetDeviceObject = ClassDriverObject->DeviceObject;

				while (TargetDeviceObject) {
					if (!TargetDeviceObject->NextDevice) {
						gMouseObject.DeviceObject = TargetDeviceObject;
						break;
					}
					TargetDeviceObject = TargetDeviceObject->NextDevice;
				}
			}
			(NT.ObfDereferenceObject)(ClassDriverObject);
			(NT.ObfDereferenceObject)(HidDriverObject);

			if (gMouseObject.DeviceObject && gMouseObject.ServiceCallback) {
				gMouseObject.UseMouse = 1;
			}

		}
		BOOLEAN result = gMouseObject.DeviceObject && gMouseObject.ServiceCallback;
		return result;

	}
	NTSTATUS MoveMouseV2(MouseMovementStruct request)
	{
		KIRQL irql;
		ULONG input_data;
		MOUSE_INPUT_DATA mid = { 0 };

		mid.LastX = request->x;
		mid.LastY = request->y;
		mid.ButtonFlags = request->button_flags;
		mid.UnitId = 1;

		irql = (NT.KeGetCurrentIrql)();
		if (!OpenMouse()) {
			return STATUS_UNSUCCESSFUL;
		}

		KeRaiseIrql(DISPATCH_LEVEL, &irql);
		MouseClassServiceCallback(gMouseObject.DeviceObject, &mid, (PMOUSE_INPUT_DATA)&mid + 1, &input_data);
		(NT.KeLowerIrql)(irql);

		return STATUS_SUCCESS;

	}
}