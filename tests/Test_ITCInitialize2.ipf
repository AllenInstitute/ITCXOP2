#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCInitialize2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCInitialize2.ipf")
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

Static Function initialize_devID_IGNORE(devID)
	variable devID

	ITCInitialize2 /DEV=(devID)

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function initialize_noDevID_IGNORE()

	ITCInitialize2

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = initialize_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = initialize_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End


// Test valid flags
Static Function MFlagValueFn_IGNORE(value)
	Variable value
	ITCInitialize2 /M=(value)

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testMFlag()

	Variable negativeOneAllowed = 1
	TestDWORDValueRange(MFlagValueFn_IGNORE, negativeOneAllowed)

	// Test valid values
	Variable expectedITCXOPError, ExpectedITCError

	expectedITCXOPError = 0
	ExpectedITCError = 0

	TestValue(MFlagValueFn_IGNORE, -1, expectedITCXOPError, ExpectedITCError)
	TestValue(MFlagValueFn_IGNORE,  0, expectedITCXOPError, ExpectedITCError)
	TestValue(MFlagValueFn_IGNORE,  1, expectedITCXOPError, ExpectedITCError)
	TestValue(MFlagValueFn_IGNORE,  2, expectedITCXOPError, ExpectedITCError)

	// There's no DLL error if you try an an out-of-range value...
	TestValue(MFlagValueFn_IGNORE,  3, expectedITCXOPError, ExpectedITCError)
End

static Function testUFlag()
	Variable error

	// No errors raised by DLL in the case of invalid filenames...
	Variable INCORRECT_FILE_ERROR = 0
	Variable EMPTY_FILE_ERROR = 0
	Variable BAD_FILENAME_ERROR = 0

	Make /T /FREE flagVals			= {"ITC18.U2F", "ITC1600.U2F",  			"",						"BAD_NAME"				}
	Make /D expectedITCXOPErrors	= {0,				0,								0,						 	0							}
	Make /D expectedITCErrors		= {0,				INCORRECT_FILE_ERROR,	EMPTY_FILE_ERROR,		BAD_FILENAME_ERROR	}

	Variable currVal
	For( currVal = 0; currVal < numpnts(flagVals); currVal += 1 )
		String		currFlagVal = flagVals[currVal]
		Variable	expectedITCError = expectedITCErrors[currVal]
		Variable	expectedITCXOPError = expectedITCXOPErrors[currVal]

		Try
			ITCInitialize2 /U=currFlagVal; AbortOnRTE
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

// We're testing with an ITC18USB.
// The flags /D, /H and /R require an ITC1600
// Check that trying to use these flags results in an error
static Function testUnsupportedFlags()
	Variable error

	Variable D_FLAG_REQUIRES_ITC1600 = 10021
	Variable H_FLAG_REQUIRES_ITC1600 = 10022
	Variable R_FLAG_REQUIRES_ITC1600 = 10023

	Try
		ITCInitialize2 /D=0; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, D_FLAG_REQUIRES_ITC1600)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry

	Try
		ITCInitialize2 /H=0; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, H_FLAG_REQUIRES_ITC1600)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry

	Try
		ITCInitialize2 /R=0; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, R_FLAG_REQUIRES_ITC1600)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry
End
