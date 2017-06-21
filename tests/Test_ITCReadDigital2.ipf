#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCReadDigital2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCReadDigital2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultChannel = 0
Static Constant		numDigitalInputs = 2; // 1 DI on device + 1 DI aux input

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

Static Function readDig_devID_IGNORE(devID)
	variable devID
	ITCReadDigital2 /DEV=(devID) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function readDig_noDevID_IGNORE()
	ITCReadDigital2 defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

// Run the standard DeviceID tests
static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = readDig_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = readDig_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

Static Function testChannelNumber_fn_IGNORE(channelNumber)
	Variable channelNumber
	ITCReadDigital2 channelNumber

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End
//
//// Test the channel number parameter
//Static Function testChannels()
//
//	Variable invalidChannelNumberError = 2154041346
//	testChannelNumbers(testChannelNumber_fn_IGNORE, numDigitalInputs, invalidChannelNumberError)
//
//End
