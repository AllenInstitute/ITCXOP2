#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCStopAcq2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCStopAcq2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to readADC from
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

// Test the /DEV flag

Static Function stopAcq_devID_IGNORE(devID)
	variable devID
	ITCStopAcq2 /DEV=(devID)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function stopAcq_noDevID_IGNORE()
	ITCStopAcq2

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = stopAcq_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = stopAcq_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Stopping acquisition functionality tested elsewhere