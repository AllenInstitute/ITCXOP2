#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=DevIDTests

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

StrConstant lastDevIDVariableName = root:lastDevNum

Function DevIDTests_devID_Prototype(devID)
	variable devID
End

Function DevIDTests_noDevID_Prototype()

End

// valuePass, valueFail are the expected values of V_Value
function devIDTest(devID_fn, noDevID_fn, [valuePass, valueFail])
	FUNCREF DevIDTests_devID_Prototype devID_fn
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn
	variable valuePass, valueFail

	variable numOpt

	numOpt = ParamIsDefault(valuePass) + ParamIsDefault(valuePass)

	switch(numOpt)
		case 0:
			// nothing to do
			break
		case 2:
			valuePass = NaN
			valueFail = NaN
			break
		default:
			FAIL()
	endswitch

	testValidDevID(devID_fn, valuePass, valueFail)
	testInvalidDevIDs(devID_fn, valuePass, valueFail)

	testLastDevID(noDevID_fn, valuePass, valueFail)
	testInvalidLastDevID(noDevID_fn, valuePass, valueFail)
End

Static Function testValidDevID(devID_fn, valuePass, valueFail)
	FUNCREF DevIDTests_devID_Prototype devID_fn
	variable valuePass, valueFail

	NVAR devNum = $lastDevIDVariableName
	devID_fn(devNum)

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)

	NVAR/Z last_Value
	if(numType(valuePass) == 2 && numType(valueFail) == 2)
		// V_Value is not expected
		CHECK(!NVAR_Exists(last_Value))
	else
		CHECK(NVAR_Exists(last_Value))
		CHECK_EQUAL_VAR(last_Value, valuePass)
	endif
End


Static Function testInvalidDevIDs(devID_fn, valuePass, valueFail)
	FUNCREF DevIDTests_devID_Prototype devID_fn
	variable valuePass, valueFail

	variable error

	Make /FREE /D /N=5 invalidDevIDs, ITCXOPErrorCode, ITCErrorCode

	// Invalid device IDs to try
	invalidDevIDs[0] = NaN
	invalidDevIDs[1] = inf
	invalidDevIDs[2] = -1
	invalidDevIDs[3] = 1024
	invalidDevIDs[4] = 5

	// Expected ITCXOPErrorCode
	ITCXOPErrorCode[0] = 863 // NaN or inf
	ITCXOPErrorCode[1] = 863 // NaN or inf
	ITCXOPErrorCode[2] = 823 // Parameter is out of range
	ITCXOPErrorCode[3] = 10004 // Unused DeviceID
	ITCXOPErrorCode[4] = 10004 // Unused DeviceID

	// Expected ITCErrorCode
	ITCErrorCode[] = 0

	Variable k
	For (k = 0; k < numpnts(invalidDevIDs); k += 1)
		try
			devID_fn(invalidDevIDs[k]); AbortOnRTE
			FAIL()
		catch
			error = GetRTError(1)
		endtry

		NVAR last_ITCXOPError, last_ITCError
		CHECK_EQUAL_VAR(last_ITCXOPError, ITCXOPErrorCode[k])
		CHECK_EQUAL_VAR(last_ITCError, ITCErrorCode[k])

		NVAR/Z last_Value
		if(numType(valuePass) == 2 && numType(valueFail) == 2)
			// V_Value is not expected
			CHECK(!NVAR_Exists(last_Value))
		else
			CHECK(NVAR_Exists(last_Value))
			CHECK_EQUAL_VAR(last_Value, valueFail)
		endif
	EndFor
End

Static Function testLastDevID(noDevID_fn, valuePass, valueFail)
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn
	variable valuePass, valueFail

	noDevID_fn()

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)

	NVAR/Z last_Value
	if(numType(valuePass) == 2 && numType(valueFail) == 2)
		// V_Value is not expected
		CHECK(!NVAR_Exists(last_Value))
	else
		CHECK(NVAR_Exists(last_Value))
		CHECK_EQUAL_VAR(last_Value, valuePass)
	endif
End

Static Function testInvalidLastDevID(noDevID_fn, valuePass, valueFail)
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn
	variable valuePass, valueFail

	Variable error

	ITCCloseAll2

	Try
		noDevID_fn(); AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 10004)
	CHECK_EQUAL_VAR(last_ITCError, 0)

	NVAR/Z last_Value
	if(numType(valuePass) == 2 && numType(valueFail) == 2)
		// V_Value is not expected
		CHECK(!NVAR_Exists(last_Value))
	else
		CHECK(NVAR_Exists(last_Value))
		CHECK_EQUAL_VAR(last_Value, valueFail)
	endif
End
