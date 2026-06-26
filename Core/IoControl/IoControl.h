#pragma once
#include "../../includes.h"

namespace IoControl
{
	NTSTATUS Dispatch(PDEVICE_OBJECT deviceObject, PIRP Irp);
}
