#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCGetCurrentDevice2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCGetDevices2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     = 0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to readADC from
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value
End

static Function testGetCurrentDevice()
	ITCGetCurrentDevice2
	CHECK_EQUAL_VAR(V_Value, 0)
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)
End

static Function testGetCurrentDeviceFails()
	variable err

	ITCCloseAll2

	try
		ITCGetCurrentDevice2; AbortONRTE
		FAIL()
	catch
		err = GetRTError(1)
		PASS()
	endtry

	CHECK_EQUAL_VAR(V_Value, -1)
	CHECK_EQUAL_VAR(V_ITCXOPError, 10004)
	CHECK_EQUAL_VAR(V_ITCError, 0)
End
