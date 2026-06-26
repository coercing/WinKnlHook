#pragma once

// ============================================================================
// Comprehensive Warning Suppression for Kernel Driver Development
// ============================================================================

// Disable all macro redefinition warnings
#pragma warning(disable: 4005)  // macro redefinition (_CRT_STRINGIZE, _CRT_WIDE, __CRTDECL, CONTAINING_RECORD)

// Disable signed/unsigned mismatch warnings
#pragma warning(disable: 4018)  // signed/unsigned mismatch

// Disable nameless struct/union warnings (common in Windows kernel structures)
#pragma warning(disable: 4201)  // nonstandard extension used: nameless struct/union

// Disable data conversion warnings
#pragma warning(disable: 4244)  // conversion from 'type1' to 'type2', possible loss of data

// Disable pointer truncation warnings
#pragma warning(disable: 4311)  // pointer truncation from 'type1' to 'type2'
#pragma warning(disable: 4302)  // truncation from 'type1' to 'type2'

// Disable unreferenced parameter warnings
#pragma warning(disable: 4100)  // unreferenced formal parameter

// Disable missing return value warnings
#pragma warning(disable: 4715)  // not all control paths return a value

// Disable pragma warnings
#pragma warning(disable: 4083)  // expected ')'; found identifier

// Disable VCRuntime warnings
#pragma warning(disable: VCR001) // Function definition not found
