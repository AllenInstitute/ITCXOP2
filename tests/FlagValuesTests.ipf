#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=TestFlagValues

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

Function noFlagFn_Prototype()

End

Function flagOnlyFn_Prototype()

End

Function flagValueFn_Prototype(value)
	Variable value
End

Function testValueFn_Prototype(value)
	Variable value
End

Function TestNoFlags(noFlagFn)
	Funcref noFlagFn_Prototype noFlagFn

	noFlagFn()
	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)
End

Function TestFlagOnly(flagOnlyFn)
	Funcref flagOnlyFn_Prototype flagOnlyFn

	flagOnlyFn()
	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)
End

// Boolean flags are defined as those which can be called as:
// [no flag], /N, /N=0, /N=1
//
// It is assumed that the "no flag" case will be tested elsewhere, since it only needs to be tested once for all functions.
// This leaves us with the case of /N and /N=k
Function TestBooleanFlag(flagOnlyFn, flagValueFn, negativeOneAllowed)
	Funcref flagOnlyFn_Prototype flagOnlyFn
	Funcref flagValueFn_Prototype flagValueFn
	Variable negativeOneAllowed

	variable kParameterOutOfRange = 823

	Variable expectedITCXOPError, expectedITCError

	// Test flag only, /N
	TestFlagOnly(flagOnlyFn)

	// Test that /N=0 and /N=1 are valid
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(flagValueFn, 0, expectedITCXOPError, expectedITCError)
	TestValue(flagValueFn, 1, expectedITCXOPError, expectedITCError)

	// NaN / inf
	TestNanInf(flagValueFn)
End

Function TestDWORDValueRange(testValueFn, negativeOneAllowed)
	Funcref testValueFn_Prototype testValueFn
	Variable negativeOneAllowed

	variable kParameterOutOfRange = 823

	Variable expectedITCXOPError, expectedITCError

	// Assume 0 is always valid
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(testValueFn, 0, expectedITCXOPError, expectedITCError)

	// Smallest +ve unsupported value
	Variable DWORD_MAX = 4294967295
	expectedITCXOPError = kParameterOutOfRange
	expectedITCError = 0
	TestValue(testValueFn, DWORD_MAX+1, expectedITCXOPError, expectedITCError)

	// Largest -ve unsupported value.
	// Sometimes -1 is special cased as "do not change"

	// -1
	If( negativeOneAllowed )
		expectedITCXOPError = 0
		expectedITCError = 0
	Else
		expectedITCXOPError = kParameterOutOfRange
		expectedITCError = 0
	EndIf
	TestValue(testValueFn, -1, expectedITCXOPError, expectedITCError)

	// NaN / inf
	TestNanInf(testValueFn)
End

Function TestValue(testValueFn, value, expectedITCXOPError, expectedITCError)
	Funcref testValueFn_Prototype testValueFn
	Variable value, expectedITCXOPError, expectedITCError

	variable error

	Try
		testValueFn(value); AbortOnRTE

		// Fail if we expected an error and didn't get one
		If( expectedITCXOPError != 0)
			FAIL()
		EndIf
	Catch
		error = GetRTError(1)

		// Fail if we didn't expect an error, but encountered one
		If( expectedITCXOPError == 0)
			FAIL()
		EndIf
	EndTry

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, expectedITCXOPError)
	CHECK_EQUAL_VAR(last_ITCError, expectedITCError)
End

Function TestNanInf(testValueFn)
	Funcref testValueFn_Prototype testValueFn

	Variable kDoesNotSupportNaNorINF	= 863

	Variable expectedITCXOPError = kDoesNotSupportNaNorINF
	Variable expectedITCError = 0

	TestValue(testValueFn, NaN, expectedITCXOPError, expectedITCError)
	TestValue(testValueFn, Inf, expectedITCXOPError, expectedITCError)
	TestValue(testValueFn, -Inf, expectedITCXOPError, expectedITCError)
End

Function testChannelNumbers(testChannelNumberFn, numValidChannels, expectedDLLError)
	Funcref testValueFn_Prototype testChannelNumberFn
	Variable numValidChannels, expectedDLLError

	Variable expectedITCXOPError, expectedITCError

	// Test valid channel numbers
	expectedITCXOPError = 0
	expectedITCError = 0

	Variable currChannel
	For( currChannel = 0; currChannel < numValidChannels; currChannel += 1)
		TestValue(testChannelNumberFn, currChannel, expectedITCXOPError, expectedITCError)
	EndFor

	// Test invalid DWORD channel numbers
	Variable negativeOneAllowed = 0
	TestDWORDValueRange(testChannelNumberFn, negativeOneAllowed)

	// Test smallest invalid channel number
	If( expectedDLLError == 0 )
		// Don't deliberately pass invalid values that we know won't give an error...
		return 0
	EndIf

	expectedITCXOPError = 10006
	expectedITCError = expectedDLLError

	TestValue(testChannelNumberFn, numValidChannels+1, expectedITCXOPError, expectedITCError)
End
