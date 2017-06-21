#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCGetAllChannelsConfig2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCGetAllChannelsConfig2.ipf")
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

Static Function getAllConf_devID_IGNORE(devID)
	variable devID

	ITCGetAllChannelsConfig2 /FREE /DEV=(devID) *, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getAllConf_noDevID_IGNORE()

	ITCGetAllChannelsConfig2 /FREE *, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = getAllConf_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = getAllConf_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End


// Test the channel selection wave
Static Function testFn_1ch(selectionWave)
	WAVE selectionWave

	ITCGetAllChannelsConfig2 /FREE selectionWave, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testFn_2ch(selectionWave)
	WAVE selectionWave

	ITCGetAllChannelsConfig2 /FREE selectionWave, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function test_confWave()
	Wave configWave1ch = makeChannelSelectionWave(1)
	Wave configWave2ch = makeChannelSelectionWave(2)
	configWave2ch[1][1] = 1

	testSelectionWave(testFn_1ch, testFn_2ch, configWave1ch, configWave2ch)
End

// Test with * selection / out wave
Static Function test_null()
	variable error

	// Using * as the selection wave should be OK
	// It ought to select all channels.
	ITCGetAllChannelsConfig2 /FREE *, out

	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	// Igor won't let * be used as a dataFolderAndName parameter
End

Static Function testOutput()

	ITCGetAllChannelsConfig2 /FREE *, out

	// TODO -- how to test that the output is OK?
End

Static Function checkDimensionLabels()
	ITCGetAllChannelsConfig2 /FREE *, outWave

	Variable dimension = 0
	Make /FREE /T desiredDimLabels = {"", "ChannelType", "ChannelNumber", "", "ErrorMode", "ErrorState", "FIFOPointer", "FIFONumberOfPoints", "", "", "", "SamplingIntervalFlag", "SamplingRate", "StartOffset", "Gain", "Offset", "Decimation", "", "", ""}
	WAVE skipWave

	CheckDimLabels(dimension, outWave, desiredDimLabels, skipWave)
End

Static Function DestTest()
	variable error
	String outWaveName = "outWave_string"
	// String freeOutWaveName = "freeOutWave_string"
	Variable NAME_WAV_CONFLICT = 27

//------------------------------------------
// Create waves

// Normal wave
	 ITCGetAllChannelsConfig2 *, outWave

// Wave name
	 ITCGetAllChannelsConfig2 *, $outWaveName

//------------------------------------------
// Overwrite -- fail if /O flag not specified
	try
		 ITCGetAllChannelsConfig2 *, outWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

	try
		 ITCGetAllChannelsConfig2 *, $outWaveName; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

//------------------------------------------
// Overwrite -- OK if /O flag specified
	 ITCGetAllChannelsConfig2 /O *, outWave
	 ITCGetAllChannelsConfig2 /O *, $outWaveName

//------------------------------------------
// Free wave
	 ITCGetAllChannelsConfig2 /FREE *, freeOutWave
	CHECK_EQUAL_VAR(WaveType(freeOutWave, 2), 2)

	// So, it seems combining /FREE with string-to-wave reference $ doesn't work properly.
	// Trying to call
	//    Make /FREE $waveName
	// gives an error before compilation.
	//
	// Not sure why we don't get an error before compilation here...

	//  ITCGetAllChannelsConfig2 /FREE *, $freeOutWaveName
	// CHECK_EQUAL_VAR(WaveType(freeOutWave2, 2), 2)

//------------------------------------------
// Free wave overwrites don't fail
// (the wave reference is updated to the new wave)

	 ITCGetAllChannelsConfig2 /FREE *, freeOutWave
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	//  ITCGetAllChannelsConfig2 /FREE *, $freeOutWaveName
	// CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	// CHECK_EQUAL_VAR(V_ITCError, 0)

//------------------------------------------
// Free wave overwrite should also work if the /O flag specified
	 ITCGetAllChannelsConfig2 /O /FREE *, freeOutWave
	//  ITCGetAllChannelsConfig2 /O /FREE *, $freeOutWaveName
End
