#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCWriteDigital2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCWriteDigital2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultChannel = 0
Static Constant		numDigitalOutputs = 4; // 2 DO + 2 aux outputs
Static Constant		defaultValue =0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to setDAC on
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

// Test the /DEV flag

Static Function writeDig_devID_IGNORE(devID)
	variable devID
	ITCWriteDigital2 /DEV=(devID) defaultChannel, defaultValue

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function writeDig_noDevID_IGNORE()
	ITCWriteDigital2 defaultChannel, defaultValue

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = writeDig_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = writeDig_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the channel number parameter
Static Function testChannelNumber_fn_IGNORE(channelNumber)
	Variable channelNumber
	ITCWriteDigital2 channelNumber, defaultValue

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

//Static Function testChannels()
//
//	Variable invalidChannelNumberError = 2154041346
//	testChannelNumbers(testChannelNumber_fn_IGNORE, numDigitalOutputs, invalidChannelNumberError)
//End


Static Function testValue_fn_IGNORE(value)
	Variable value
	ITCWriteDigital2 defaultChannel, value

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

// Test the output value parameter
Static Function testValues()
	Variable expectedITCXOPError, expectedITCError

	// Test smallest/largest valid values
	expectedITCXOPError = 0
	expectedITCError = 0

	TestValue(testValue_fn_IGNORE, 0, 		expectedITCXOPError, expectedITCError)
	TestValue(testValue_fn_IGNORE, 65535,	expectedITCXOPError, expectedITCError)

	// Test Nan/Inf
	TestNanInf(testChannelNumber_fn_IGNORE)

	// Test closest values outside range
	expectedITCXOPError = 823 // Value out of range
	expectedITCError = 0

	TestValue(testValue_fn_IGNORE, -1, 		expectedITCXOPError, expectedITCError)
	TestValue(testValue_fn_IGNORE, 65536, 	expectedITCXOPError, expectedITCError)
End
