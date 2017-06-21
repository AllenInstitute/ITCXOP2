#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCConfigChannelUpload2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCConfigChannelUpload2.ipf")
// and disable debugger

Static StrConstant	defaultWaveName = "root:fifoWAVE"

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to config
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	// Set a config so we can upload it
	Make /O /W $defaultWaveName
	ITCConfigChannel2 /CHN=0 0, 100, $defaultWaveName

	TEST_CASE_BEGIN(name)
End

Static Function devID_IGNORE(devID)
	variable devID
	ITCConfigChannelUpload2 /DEV=(devID)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function noDevID_IGNORE()
	ITCConfigChannelUpload2

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannelUpload_test_devID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the DLL error which is raised when no config is supplied prior to upload
Static Function test_DLLError()
	Variable error
	NVAR devID = $lastDevIDVariableName

// Clear the config
	ITCConfigChannelReset2 /DEV=(devID)

// Attempt to upload
	Try
		ITCConfigChannelUpload2; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 0)
		CHECK_EQUAL_VAR(V_ITCError, 2154827777)
	EndTry

	Try
		ITCConfigChannelUpload2 /DEV=(devID); AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 0)
		CHECK_EQUAL_VAR(V_ITCError, 2154827777)
	EndTry

// Close and re-open the device
// This also re-sets the configuration.
	ITCCloseDevice2 /DEV=(devID)
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	devID = V_Value

// Try to upload again
	Try
		ITCConfigChannelUpload2; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 0)
		CHECK_EQUAL_VAR(V_ITCError, 2154827777)
	EndTry

	Try
		ITCConfigChannelUpload2 /DEV=(devID); AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 0)
		CHECK_EQUAL_VAR(V_ITCError, 2154827777)
	EndTry

End
