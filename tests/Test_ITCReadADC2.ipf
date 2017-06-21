#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCReadADC2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCReadADC2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultChannel = 0
Static Constant		numADCChannels = 8

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

Static Function readADC_devID_IGNORE(devID)
	variable devID
	ITCReadADC2 /DEV=(devID) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function readADC_noDevID_IGNORE()
	ITCReadADC2 defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = readADC_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = readADC_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the channel number parameter
Static Function testChannelNumberFn_IGNORE(channelNumber)
	Variable channelNumber
	ITCReadADC2 channelNumber

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

//Static Function testChannels()
//
//	// It seems that this function doesn't discriminate between ADC and DI channels.
//	// So it *DOESN'T* give an error if you try to read from a DI channel...
//	Variable numDigitalInputs = 2
//	Variable totalNumInputChannels = numADCChannels + numDigitalInputs
//
//	Variable expectedDLLError = 2154041346
//	testChannelNumbers(testChannelNumberFn_IGNORE, totalNumInputChannels, expectedDLLError)
//
//End

// Test the various flags for error conditions.
// Testing the actual use of the flags occurs elsewhere (along with ITCSetDAC2)

Static Function noFlagFn_IGNORE()
	ITCReadADC2 defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function CflagOnlyFn_IGNORE()
	ITCReadADC2 /C defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function CflagValueFn_IGNORE(value)
	Variable value

	ITCReadADC2 /C=(value) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function VflagOnlyFn_IGNORE()
	ITCReadADC2 /V defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function VflagValueFn_IGNORE(value)
	Variable value

	ITCReadADC2 /V=(value) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End


Static Function testFlags()
	Variable negativeOneAllowed

	// Test the case of no flags
	TestNoFlags(noFlagFn_IGNORE)

	// Test C and V boolean flags
	negativeOneAllowed = 0
	TestBooleanFlag(CflagOnlyFn_IGNORE, CflagValueFn_IGNORE, negativeOneAllowed)
	TestBooleanFlag(VflagOnlyFn_IGNORE, VflagValueFn_IGNORE, negativeOneAllowed)
End

// TODO:
// - Test /C flag:
// 		-> No flag or /C or /C=1 --> Calibrate
//		-> /C=0 --> Don't calibrate

// TODO:
// - Test /V flag
//		-> No flag or /V=0 --> Don't convert to volts
//		-> /V or /V=1 --> Convert to volts
