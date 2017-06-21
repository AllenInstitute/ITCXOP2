#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCSetDAC2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCSetDAC2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultChannel = 0
Static Constant		numDACChannels = 4
Static Constant		defaultVoltage =0

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

Static Function setDAC_devID_IGNORE(devID)
	variable devID
	ITCSetDAC2 /DEV=(devID) defaultChannel, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function setDAC_noDevID_IGNORE()
	ITCSetDAC2 defaultChannel, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = setDAC_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = setDAC_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the channel number parameter
Static Function testChannelNumberFn_IGNORE(channelNumber)
	Variable channelNumber
	ITCSetDAC2 channelNumber, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

//Static Function testChannels()
//
//	// It seems that this function doesn't discriminate between DAC and DO channels.
//	// So it *DOESN'T* give an error if you try to write to a DO channel...
//	Variable numDigitalOutputs = 4
//	Variable totalNumOutputChannels = numDACChannels + numDigitalOutputs
//
//	Variable expectedDLLError = 2154041346
//	testChannelNumbers(testChannelNumberFn_IGNORE, totalNumOutputChannels, expectedDLLError)
//
//End

// Test the various flags for error conditions.
// Testing the actual use of the flags occurs elsewhere (along with ITCReadADC2)
Static Function noFlagFn_IGNORE()
	ITCReadADC2 defaultChannel

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function CflagOnlyFn_IGNORE()
	ITCSetDAC2 /C defaultChannel, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function CflagValueFn_IGNORE(value)
	Variable value

	ITCSetDAC2 /C=(value) defaultChannel, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function VflagOnlyFn_IGNORE()
	ITCSetDAC2 /V defaultChannel, defaultVoltage

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function VflagValueFn_IGNORE(value)
	Variable value

	ITCSetDAC2 /V=(value) defaultChannel, defaultVoltage

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

// Test setting voltage
Static Function testVoltage()
	Variable error

	Variable short_min = -32768
	Variable short_max = 32767
	Variable ANALOG_VOLT = 3200

	// Voltage values
	// Test a load of valid values, and some invalid ones.

	Make /D VoltageVals_DACUnits	= {0,	20,	-20,	5 * ANALOG_VOLT,	-5 * ANALOG_VOLT,	short_min,	short_max,	short_min - 1,	short_max + 1,	inf,	-inf,	NaN	}
	Make /D expectedITCXOPErrors	= {0,	0,		0,		0,						0,						0,				0,				823,				823,				863,	863,	863	}
	Make /D expectedITCErrors		= {0,	0,		0,		0,						0,						0,				0,				0,					0,					0,		0,		0	}
	Duplicate VoltageVals_DACUnits, VoltageVals_V
	VoltageVals_V /= ANALOG_VOLT


	Variable currVal
	For( currVal = 0; currVal < numpnts(VoltageVals_DACUnits); currVal += 1 )

		Variable currVal_DACUnits = VoltageVals_DACUnits[currVal]
		Variable currVal_V = VoltageVals_V[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]

		//----------------------------------------------------------
		// Test without flag
		//
		Try
			ITCSetDAC2 defaultChannel, currVal_V; AbortOnRTE
			// If we expected an error, but didn't get one, FAIL()
			If( expectedITCError != 0 || expectedITCXOPError != 0 )
				FAIL()
			EndIf
		Catch
			error = GetRTError(1)
			// If we didn't expect an error, FAIL()
			If( expectedITCError == 0 && expectedITCXOPError == 0 )
				FAIL()
			EndIf
		EndTry

		CHECK_EQUAL_VAR(V_ITCXOPError, expectedITCXOPError)
		CHECK_EQUAL_VAR(V_ITCError, expectedITCError)

		//----------------------------------------------------------
		// Test with /V=0
		//
		Try
			ITCSetDAC2 /V=0 defaultChannel, currVal_DACUnits; AbortOnRTE
			// If we expected an error, but didn't get one, FAIL()
			If( expectedITCError != 0 || expectedITCXOPError != 0 )
				FAIL()
			EndIf
		Catch
			error = GetRTError(1)
			// If we didn't expect an error, FAIL()
			If( expectedITCError == 0 && expectedITCXOPError == 0 )
				FAIL()
			EndIf
		EndTry

		CHECK_EQUAL_VAR(V_ITCXOPError, expectedITCXOPError)
		CHECK_EQUAL_VAR(V_ITCError, expectedITCError)

		//----------------------------------------------------------
		// Test with /V
		//
		Try
			ITCSetDAC2 /V defaultChannel, currVal_V; AbortOnRTE
			// If we expected an error, but didn't get one, FAIL()
			If( expectedITCError != 0 || expectedITCXOPError != 0 )
				FAIL()
			EndIf
		Catch
			error = GetRTError(1)
			// If we didn't expect an error, FAIL()
			If( expectedITCError == 0 && expectedITCXOPError == 0 )
				FAIL()
			EndIf
		EndTry

		CHECK_EQUAL_VAR(V_ITCXOPError, expectedITCXOPError)
		CHECK_EQUAL_VAR(V_ITCError, expectedITCError)

		//----------------------------------------------------------
		// Test with /V=1
		//
		Try
			ITCSetDAC2 /V=1 defaultChannel, currVal_V; AbortOnRTE
			// If we expected an error, but didn't get one, FAIL()
			If( expectedITCError != 0 || expectedITCXOPError != 0 )
				FAIL()
			EndIf
		Catch
			error = GetRTError(1)
			// If we didn't expect an error, FAIL()
			If( expectedITCError == 0 && expectedITCXOPError == 0 )
				FAIL()
			EndIf
		EndTry

		CHECK_EQUAL_VAR(V_ITCXOPError, expectedITCXOPError)
		CHECK_EQUAL_VAR(V_ITCError, expectedITCError)
	EndFor

End

// TODO:
// - Test /C flag:
// 		-> No flag or /C or /C=1 --> Calibrate
//		-> /C=0 --> Don't calibrate

// TODO:
// - Test /V flag
//		-> No flag or /V=0 --> Don't convert to volts
//		-> /V or /V=1 --> Convert to volts
