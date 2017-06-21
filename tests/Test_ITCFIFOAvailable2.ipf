#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCFIFOAvailable2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCFIFOAvailable2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultCHN = 0
Static StrConstant		defaultCHS = "IN"
Static Constant		defaultChannel = 0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device open
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

// Test CHN / CHS
Static Function fifoAvailable_chn_fn_IGNORE(chn, channelNumber)
	variable chn, channelNumber

	ITCFIFOAvailable2 /CHN=(chn) channelNumber

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function fifoAvailable_chs_fn_IGNORE(chs, channelNumber)
	string chs
	variable channelNumber

	ITCFIFOAvailable2 /CHS=chs channelNumber

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function fifoAvailable_chn_chs_fn_IGNORE(chn, chs, channelNumber)
	variable chn
	string chs
	variable channelNumber

	ITCFIFOAvailable2 /CHN=(chn)/CHS=chs channelNumber

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function fifoAvailable_none_fn_IGNORE(channelNumber)
	variable channelNumber

	ITCFIFOAvailable2 channelNumber

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Function fifoAvailable_test_chn_chs()
	FUNCREF chn_fn_proto chn_fn = fifoAvailable_chn_fn_IGNORE
	FUNCREF chs_fn_proto chs_fn = fifoAvailable_chs_fn_IGNORE
	FUNCREF chn_chs_fn_proto chn_chs_fn = fifoAvailable_chn_chs_fn_IGNORE
	FUNCREF none_fn_proto none_fn = fifoAvailable_none_fn_IGNORE

// The call underlying ITCFIFOAvailable2 doesn't appear to give a DLL error if you try to access an invalid channel...
	Variable BAD_CHANNEL_NUMBER_DLL_ERROR = 0
	Variable SKIP_CHANNEL = NaN

	chn_chs_test(chn_fn, chs_fn, chn_chs_fn, none_fn, SKIP_CHANNEL, BAD_CHANNEL_NUMBER_DLL_ERROR)
End

Static Function fifoAvailable_devID_IGNORE(devID)
	variable devID
	ITCFifoAvailable2 /DEV=(devID) /CHN=(defaultCHN) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function fifoAvailable_noDevID_IGNORE()
	ITCFifoAvailable2 /CHN=(defaultCHN) defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function configChannel_test_devID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = fifoAvailable_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = fifoAvailable_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

Static Function testOutput()
	// Check that V_Value is set properly.

	// Get the number of points
	ITCFifoAvailable2 /CHN=(defaultCHN) defaultChannel

	// Check that the number of points is as expected
	// TODO: Not started acquisition, so the number of points should be zero? Change if required.
	CHECK_EQUAL_VAR(V_Value, 0)

	ITCCloseAll2

End
