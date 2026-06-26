#pragma once
#include "../../Includes.h"

typedef struct _movemouse {
	long x;
	long y;
	unsigned short button_flags;
} movemouse, * MouseMovementStruct;

namespace mousemove
{
	VOID InitializeMouseControl(VOID);
	BOOLEAN OpenMouse(VOID);
	NTSTATUS MoveMouseV2(MouseMovementStruct request);
}