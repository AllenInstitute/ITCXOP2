#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=DevIDTests

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

StrConstant lastDevIDVariableName = root:lastDevNum

Function DevIDTests_devID_Prototype(devID)
	variable devID
End

Function DevIDTests_noDevID_Prototype()

End

function devIDTest(devID_fn, noDevID_fn)
	FUNCREF DevIDTests_devID_Prototype devID_fn
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn

	testValidDevID(devID_fn)
	testInvalidDevIDs(devID_fn)

	testLastDevID(noDevID_fn)
	testInvalidLastDevID(noDevID_fn)
End

Static Function testValidDevID(devID_fn)
	FUNCREF DevIDTests_devID_Prototype devID_fn

	NVAR devNum = $lastDevIDVariableName
	devID_fn(devNum)

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)
End


Static Function testInvalidDevIDs(devID_fn)
	FUNCREF DevIDTests_devID_Prototype devID_fn

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
	EndFor
End

Static Function testLastDevID(noDevID_fn)
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn

	noDevID_fn()

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)
End

Static Function testInvalidLastDevID(noDevID_fn)
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn

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
End
