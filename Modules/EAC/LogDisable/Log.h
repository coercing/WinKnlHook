#pragma once
#include "../../../Includes.h"

namespace EAC
{
	namespace LogHook
	{
		NTSTATUS DisableEACLogHook();
		bool EnableEACLogHook();
		bool is_eac_caller();
	}
}