#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCReadTimer2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCReadTimer2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultMode = 1

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

Static Function readTimer_devID_IGNORE(devID)
	variable devID

	ITCReadTimer2 /FREE /DEV=(devID) defaultMode, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function readTimer_noDevID_IGNORE()

	ITCReadTimer2 /FREE defaultMode, outWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = readTimer_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = readTimer_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

Static StrConstant modeTestOutWaveName = "modeTestOutWave"

Static Function Mode_testValueFn_IGNORE(modeValue)
	Variable modeValue
	ITCReadTimer2 /O modeValue, $modeTestOutWaveName

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testMode()
	variable error

	// Check that NaN and inf are not allowed
	TestNanInf(Mode_testValueFn_IGNORE)


	// Can't use the standard testing framework because 0 is not a valid mode value.

	Make /D /FREE modeVals			= {1, 2, 3,	0, 		-1, 	4	}
	Make /D expectedITCXOPErrors	= {0, 0, 0,	823,	823,	823	}
	Make /D expectedITCErrors		= {0, 0, 0,	0,		0,		0	}

	Variable currVal
	For( currVal = 0; currVal < numpnts(modeVals); currVal += 1 )
		Variable currModeVal = modeVals[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]

		TestValue(Mode_testValueFn_IGNORE, currModeVal, expectedITCXOPError,  expectedITCError)

		Wave outWave = $modeTestOutWaveName

		// Check the outputs based on the mode
		Switch (currModeVal)
			Case 1:
				// First timer should be read
				CHECK_NEQ_VAR(outWave[0], 0)
				CHECK_EQUAL_VAR(outWave[1], 0)
				break
			Case 2:
				// Second timer should be read
				CHECK_EQUAL_VAR(outWave[0], 0)
				CHECK_NEQ_VAR(outWave[1], 0)
				break
			Case 3:
				// Both timers should be read
				CHECK_NEQ_VAR(outWave[0], 0)
				CHECK_NEQ_VAR(outWave[1], 0)
				break
		EndSwitch
	EndFor
End

Static Function checkDimensionLabels()
	ITCReadTimer2 /FREE defaultMode, outWave

	Variable dimension = 0
	Make /T desiredDimLabels = {"TotalSeconds", "RunSeconds"}
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
	ITCReadTimer2 defaultMode, outWave

// Wave name
	ITCReadTimer2 defaultMode, $outWaveName

//------------------------------------------
// Overwrite -- fail if /O flag not specified
	try
		ITCReadTimer2 defaultMode, outWave; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

	try
		ITCReadTimer2 defaultMode, $outWaveName; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	endtry

//------------------------------------------
// Overwrite -- OK if /O flag specified
	ITCReadTimer2 /O defaultMode, outWave
	ITCReadTimer2 /O defaultMode, $outWaveName

//------------------------------------------
// Free wave
	ITCReadTimer2 /FREE defaultMode, freeOutWave
	CHECK_EQUAL_VAR(WaveType(freeOutWave, 2), 2)

	// So, it seems combining /FREE with string-to-wave reference $ doesn't work properly.
	// Trying to call
	//    Make /FREE $waveName
	// gives an error before compilation.
	//
	// Not sure why we don't get an error before compilation here...

	// ITCReadTimer2 /FREE defaultMode, $freeOutWaveName
	// CHECK_EQUAL_VAR(WaveType(freeOutWave2, 2), 2)

//------------------------------------------
// Free wave overwrites don't fail
// (the wave reference is updated to the new wave)

	ITCReadTimer2 /FREE defaultMode, freeOutWave
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	// ITCReadTimer2 /FREE defaultMode, $freeOutWaveName
	// CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	// CHECK_EQUAL_VAR(V_ITCError, 0)

//------------------------------------------
// Free wave overwrite should also work if the /O flag specified
	ITCReadTimer2 /O /FREE defaultMode, freeOutWave
	// ITCReadTimer2 /O /FREE defaultMode, $freeOutWaveName
End
