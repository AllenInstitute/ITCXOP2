#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCConfigAllChannels2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCConfigAllChannels2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0
Static StrConstant configWaveName = "root:configWave"
Static StrConstant fifoWaveName = "root:fifoWave"
Static StrConstant fifoWave2chName = "root:fifoWave2Ch"

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to readADC from
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value


	// Make a global config wave
	Duplicate /O makeConfigAllWave(1), $configWaveName
	WAVE configWave = $configWaveName
	configWave[%SamplingInterval] = 100

	if( !WaveExists($fifoWaveName) )
		// Make a global fifoWave
		Duplicate /O makeFIFOWave(100,1), $fifoWaveName
	EndIf

	if( !WaveExists($fifoWave2chName) )
		// Make a global fifoWave
		Duplicate /O makeFIFOWave(100,2), $fifoWave2chName
	EndIf

	TEST_CASE_BEGIN(name)
End

// Test the /DEV flag

Static Function confAll_devID_IGNORE(devID)
	variable devID
	Wave configWave = $configWaveName
	Wave fifoWave = $fifoWaveName

	ITCConfigAllChannels2 /DEV=(devID) configWave, fifoWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function confAll_noDevID_IGNORE()

	Wave configWave = $configWaveName
	Wave fifoWave = $fifoWaveName

	ITCConfigAllChannels2 configWave, fifoWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = confAll_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = confAll_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the config wave
Static Function testFn_1ch(selectionWave)
	WAVE selectionWave

	Wave fifoWave = $fifoWaveName
	ITCConfigAllChannels2 selectionWave, fifoWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testFn_2ch(selectionWave)
	WAVE selectionWave

	Wave fifoWave2ch = $fifoWave2chName
	ITCConfigAllChannels2 selectionWave, fifoWave2ch

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function test_confWave()
	Wave configWave1ch = makeConfigAllWave(1)
	Wave configWave2ch = makeConfigAllWave(2)

	configWave1ch[%SamplingInterval][] = 100
	configWave2ch[%SamplingInterval][] = 100
	configWave2ch[%ChannelType][1] = 1

	testSelectionWave(testFn_1ch, testFn_2ch, configWave1ch, configWave2ch)
End

Static Function test_null()
	// Test with * config / FIFO wave
	variable error

	Variable NULL_WAVE_OP = 330

	Wave configWave = $configWaveName
	Wave fifoWave = $fifoWaveName


	Try
		ITCConfigAllChannels2 *, fifoWave; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry

	CHECK_EQUAL_VAR(V_ITCXOPError, NULL_WAVE_OP)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	Try
		ITCConfigAllChannels2 configWave, *; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry

	CHECK_EQUAL_VAR(V_ITCXOPError, NULL_WAVE_OP)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	Try
		ITCConfigAllChannels2 *, *; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry

	CHECK_EQUAL_VAR(V_ITCXOPError, NULL_WAVE_OP)
	CHECK_EQUAL_VAR(V_ITCError, 0)
End

// Test error conditions for the FIFO wave
//
// Valid FIFO waves tested as part of test_confWave()
Static Function test_fifoWave_invalid()
	variable error

	Variable NT_INCOMPATIBLE = 16
	Variable EXPECT_MATRIX = 408
	Variable INCOMPATIBLE_DIMENSIONING = 444

	Wave configWave = $configWaveName


	// Wrong type
	Make /FREE /N=(100,1) fifoWaveBadType
	try
		ITCConfigAllChannels2 configWave, fifoWaveBadType; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NT_INCOMPATIBLE)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry

	// 3D wave
	Make /FREE /W /N=(4,1,1) fifoWave
	try
		ITCConfigAllChannels2 configWave, fifoWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, EXPECT_MATRIX)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry


	// Too many columns in FIFO wave, compared to configWave
	Make /FREE /W /N=(100,2) fifoWave
	try
		ITCConfigAllChannels2 configWave, fifoWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, INCOMPATIBLE_DIMENSIONING)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry

	// Too few columns in FIFO wave, compared to configWave
	WAVE configWave2ch = makeConfigAllWave(2)
	Make /FREE /W /N=(100,1) fifoWave
	try
		ITCConfigAllChannels2 configWave2ch, fifoWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, INCOMPATIBLE_DIMENSIONING)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry
End
