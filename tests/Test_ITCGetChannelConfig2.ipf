#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCGetChannelConfig2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCGetChannelConfig2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant defaultCHN     = 0
Static Constant defaultChannel = 0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to config
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

// Test the /DEV flag

Static Function getConfig_devID_IGNORE(devID)
	variable devID
	ITCGetChannelConfig2 /FREE /DEV=(devID) /CHN=(defaultCHN) defaultChannel, out

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getConfig_noDevID_IGNORE()
	ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, out


	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = getConfig_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = getConfig_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test CHN and CHS
/// Check all valid combinations of CHN and channelNumber do not produce an error
/// NB -- assuming ITC18USB

Static Function getConfig_chn_fn_IGNORE(chn, channelNumber)
	variable chn, channelNumber

	ITCGetChannelConfig2 /FREE /CHN=(chn) channelNumber, out

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getConfig_chs_fn_IGNORE(chs, channelNumber)
	string chs
	variable channelNumber

	ITCGetChannelConfig2 /FREE /CHS=chs channelNumber, out

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getConfig_chn_chs_fn_IGNORE(chn, chs, channelNumber)
	variable chn
	string chs
	variable channelNumber

	ITCGetChannelConfig2 /FREE /CHN=(chn)/CHS=chs channelNumber, out

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getConfig_none_fn_IGNORE(channelNumber)
	variable channelNumber

	ITCGetChannelConfig2 /FREE channelNumber, out

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

//Static Function getConfig_test_chn_chs()
//	FUNCREF chn_fn_proto chn_fn = getConfig_chn_fn_IGNORE
//	FUNCREF chs_fn_proto chs_fn = getConfig_chs_fn_IGNORE
//	FUNCREF chn_chs_fn_proto chn_chs_fn = getConfig_chn_chs_fn_IGNORE
//	FUNCREF none_fn_proto none_fn = getConfig_none_fn_IGNORE
//
//	// So, similarly to ITCConfigChannel2
//	// PROBLEM
//	//
//	// When:
//	//   /CHN=5, channelNumber = 0
//	//   /CHS="AO", channelNumber = 0
//	// The DLL raises an error:
//	// Direction (0x80631000)
//	//
//	// At present, skipping testing /CHN=5...
//
//
//	Variable BAD_CHANNEL_NUMBER_DLL_ERROR = 2154041344
//	Variable SKIP_CHANNEL = 5
//	chn_chs_test(chn_fn, chs_fn, chn_chs_fn, none_fn, SKIP_CHANNEL, BAD_CHANNEL_NUMBER_DLL_ERROR)
//End

// Test dimension labels
Static Function checkDimensionLabels()
	ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, outWave

	Variable dimension = 0
	Make /T desiredDimLabels = {"", "ChannelType", "ChannelNumber", "", "ErrorMode", "ErrorState", "FIFOPointer", "FIFONumberOfPoints", "", "", "", "SamplingIntervalFlag", "SamplingRate", "StartOffset", "Gain", "Offset", "Decimation", "", "", "", ""}
	Make /N=(numpnts(desiredDimLabels)) skipWave
	skipWave[0] = 1
	skipWave[3] = 1
	skipWave[8,10] = 1
	skipWave[17,19] = 1

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
	ITCGetChannelConfig2 /CHN=(defaultCHN) defaultChannel, outWave

// Wave name
	ITCGetChannelConfig2 /CHN=(defaultCHN) defaultChannel, $outWaveName

//------------------------------------------
// Overwrite -- fail if /O flag not specified
	try
		ITCGetChannelConfig2 /CHN=(defaultCHN) defaultChannel, outWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

	try
		ITCGetChannelConfig2 /CHN=(defaultCHN) defaultChannel, $outWaveName; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

//------------------------------------------
// Overwrite -- OK if /O flag specified
	ITCGetChannelConfig2 /O /CHN=(defaultCHN) defaultChannel, outWave
	ITCGetChannelConfig2 /O /CHN=(defaultCHN) defaultChannel, $outWaveName

//------------------------------------------
// Free wave
	ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, freeOutWave
	CHECK_EQUAL_VAR(WaveType(freeOutWave, 2), 2)

	// So, it seems combining /FREE with string-to-wave reference $ doesn't work properly.
	// Trying to call
	//    Make /FREE $waveName
	// gives an error before compilation.
	//
	// Not sure why we don't get an error before compilation here...

	// ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, $freeOutWaveName
	// CHECK_EQUAL_VAR(WaveType(freeOutWave2, 2), 2)

//------------------------------------------
// Free wave overwrites don't fail
// (the wave reference is updated to the new wave)

	ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, freeOutWave
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	// ITCGetChannelConfig2 /FREE /CHN=(defaultCHN) defaultChannel, $freeOutWaveName
	// CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	// CHECK_EQUAL_VAR(V_ITCError, 0)

//------------------------------------------
// Free wave overwrite should also work if the /O flag specified
	ITCGetChannelConfig2 /O /FREE /CHN=(defaultCHN) defaultChannel, freeOutWave
	// ITCGetChannelConfig2 /O /FREE /CHN=(defaultCHN) defaultChannel, $freeOutWaveName
End
