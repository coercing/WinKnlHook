#include "IoControl.h"

extern uint64_t g_original_function;

typedef NTSTATUS(*OriginalDispatch)(PDEVICE_OBJECT, PIRP);

#include "IoControl.h"

namespace IoControl
{
	auto Dispatch(PDEVICE_OBJECT deviceObject, PIRP irp) -> NTSTATUS
	{
		UNREFERENCED_PARAMETER(deviceObject);
		NTSTATUS ioStatus = { 0 };
		ULONG bytesHandled = { 0 };
		PIO_STACK_LOCATION stackLocation = IoGetCurrentIrpStackLocation(irp);
		ULONG ioCode = stackLocation->Parameters.DeviceIoControl.IoControlCode;
		ULONG inputSize = stackLocation->Parameters.DeviceIoControl.InputBufferLength;
		
		if (ioCode == Base_code)
		{
			if (inputSize == sizeof(base_new)) {
				pbase_new req = (pbase_new)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = BaseAddy::BaseAddress(req);
				bytesHandled = sizeof(base_new);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		else if (ioCode == Read_code)
		{
			if (inputSize == sizeof(read)) {
				pread req = (pread)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = Read::ReadHandler(req);
				bytesHandled = sizeof(read);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		else if (ioCode == Write_code)
		{
			if (inputSize == sizeof(write)) {
				pwrite req = (pwrite)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = Write::WriteHandler(req);
				bytesHandled = sizeof(write);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		else if (ioCode == CR3_code)
		{
			if (inputSize == sizeof(dtb)) {
				pdtb req = (pdtb)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = PML4::DecryptCR3(req);
				bytesHandled = sizeof(dtb);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		else if (ioCode == Mouse_code)
		{
			if (inputSize == sizeof(movemouse)) {
				MouseMovementStruct req = (MouseMovementStruct)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = mousemove::MoveMouseV2(req);
				bytesHandled = sizeof(movemouse);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		else if (ioCode == Pattern_code)
		{
			if (inputSize == sizeof(pattern_scan)) {
				ppattern_scan req = (ppattern_scan)(irp->AssociatedIrp.SystemBuffer);
				ioStatus = PatternScan::ScanPattern(req);
				bytesHandled = sizeof(pattern_scan);
			}
			else {
				ioStatus = STATUS_INFO_LENGTH_MISMATCH;
				bytesHandled = 0;
			}
		}
		
		irp->IoStatus.Status = ioStatus;
		irp->IoStatus.Information = bytesHandled;
		(NT.IofCompleteRequest)(irp, IO_NO_INCREMENT);
		return ioStatus;
	}
}