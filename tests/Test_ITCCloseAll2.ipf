#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCCloseAll2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/byte-physics/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCCloseAll2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0
Variable devNum

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	ApplySafeGlobalDefaults()

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to close
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value
End

Static Function DoTest()
	ITCGetCurrentDevice2
	CHECK_GE_VAR(V_Value, 0)

// Test with an open device
	ITCCloseAll2
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	ITCGetCurrentDevice2/Z=1
	CHECK_EQUAL_VAR(V_Value, -1)

// Should have closed all devices.
// Test again now all devices are closed
	ITCCloseAll2
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)
End
