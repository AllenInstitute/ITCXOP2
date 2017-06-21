#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCGetState2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCGetState2.ipf")
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

Static Function getState_devID_IGNORE(devID)
	variable devID
	ITCGetState2 /ALL /FREE /DEV=(devID) outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function getState_noDevID_IGNORE()
	ITCGetState2 /ALL /FREE outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = getState_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = getState_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

static Function testFlags()
// No flags
	ITCGetState2 /FREE outWave
	NoneSet_IGNORE(outWave)

// One flag
	ITCGetState2 /ALL /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /R /FREE outWave
	RSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /OF /FREE outWave
	RNotSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /C /FREE outWave
	RNotSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /E /FREE outWave
	RNotSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

// Two flags
	ITCGetState2 /R /OF /FREE outWave
	RSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /R /C /FREE outWave
	RSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /R /E /FREE outWave
	RSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

	ITCGetState2 /OF /C /FREE outWave
	RNotSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /OF /E /FREE outWave
	RNotSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

	ITCGetState2 /C /E /FREE outWave
	RNotSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ESet_IGNORE(outWave)


	ITCGetState2 /ALL /R /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /OF /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /C /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /E /FREE outWave
	AllSet_IGNORE(outWave)

// Three flags
	ITCGetState2 /R /OF /C /FREE outWave
	RSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)

	ITCGetState2 /R /OF /E /FREE outWave
	RSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

	ITCGetState2 /R /C /E /FREE outWave
	RSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

	ITCGetState2 /OF /C /E /FREE outWave
	RNotSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ESet_IGNORE(outWave)

	ITCGetState2 /ALL /R /OF /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /R /C /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /R /E /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /O /C /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /OF /E /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /C /E /FREE outWave
	AllSet_IGNORE(outWave)

// Four flags
	ITCGetState2 /R/OF/C/E /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /R /OF /C /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /R /OF /E /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /R /C /E /FREE outWave
	AllSet_IGNORE(outWave)

	ITCGetState2 /ALL /OF /C /E /FREE outWave
	AllSet_IGNORE(outWave)

// Five flags
	ITCGetState2 /ALL /R /OF /C /E /FREE outWave
	AllSet_IGNORE(outWave)

End

// Check that the values have been set
// TODO: Can't actually check them, because 0 is a valid state
// 0 is also the value returned when the value is not read.
// Could:
// - Set un-read values to NaN
//
// For now, check that values that should not have been set are 0
Static Function NoneSet_IGNORE(outWave)
	WAVE outWave
	RNotSet_IGNORE(outWave)
	OFNotSet_IGNORE(outWave)
	CNotSet_IGNORE(outWave)
	ENotSet_IGNORE(outWave)
End

Static Function RNotSet_IGNORE(outWave)
	WAVE outWave
	CHECK_EQUAL_VAR(outWave[%RunningMode], 0)
End

Static Function OFNotSet_IGNORE(outWave)
	WAVE outWave
	CHECK_EQUAL_VAR(outWave[%Overflow], 0)
End

Static Function CNotSet_IGNORE(outWave)
	WAVE outWave
	CHECK_EQUAL_VAR(OutWave[%Clipping], 0)
End

Static Function ENotSet_IGNORE(outWave)
	WAVE outWave
	CHECK_EQUAL_VAR(OutWave[%State], 0)
End

Static Function RSet_IGNORE(outWave)
	WAVE outWave
End

Static Function OFSet_IGNORE(outWave)
	WAVE outWave
End

Static Function CSet_Ignore(outWave)
	WAVE outWave
End

Static Function ESet_IGNORE(outWave)
	WAVE outWave
	CHECK_NEQ_VAR(OutWave[%State], 0)
End

Static Function AllSet_IGNORE(outWave)
	WAVE outWave
	RSet_IGNORE(outWave)
	OFSet_IGNORE(outWave)
	CSet_IGNORE(outWave)
	ESet_IGNORE(outWave)
End

// Test dimension labels
Static Function checkDimensionLabels()
	ITCGetState2 /FREE outWave

	Variable dimension = 0
	Make /T desiredDimLabels = {"RunningMode", "Overflow", "Clipping", "State"}
	Wave skipWave

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
	ITCGetState2 /ALL  outWave

// Wave name
	ITCGetState2 /ALL  $outWaveName

//------------------------------------------
// Overwrite -- fail if /O flag not specified
	try
		ITCGetState2 /ALL  outWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

	try
		ITCGetState2 /ALL  $outWaveName; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

//------------------------------------------
// Overwrite -- OK if /O flag specified
	ITCGetState2 /O /ALL  outWave
	ITCGetState2 /O /ALL  $outWaveName

//------------------------------------------
// Free wave
	ITCGetState2 /FREE /ALL  freeOutWave
	CHECK_EQUAL_VAR(WaveType(freeOutWave, 2), 2)

	// So, it seems combining /FREE with string-to-wave reference $ doesn't work properly.
	// Trying to call
	//    Make /FREE $waveName
	// gives an error before compilation.
	//
	// Not sure why we don't get an error before compilation here...

	// ITCGetState2 /FREE /ALL  $freeOutWaveName
	// CHECK_EQUAL_VAR(WaveType(freeOutWave2, 2), 2)

//------------------------------------------
// Free wave overwrites don't fail
// (the wave reference is updated to the new wave)

	ITCGetState2 /FREE /ALL  freeOutWave
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	// ITCGetState2 /FREE /ALL  $freeOutWaveName
	// CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	// CHECK_EQUAL_VAR(V_ITCError, 0)

//------------------------------------------
// Free wave overwrite should also work if the /O flag specified
	ITCGetState2 /O /FREE /ALL  freeOutWave
	// ITCGetState2 /O /FREE /ALL  $freeOutWaveName
End
