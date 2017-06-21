#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// Functions to test channel selection waves.
/// Test for:
/// - Bad type
/// - Too few rows
/// - Undesired 3D wave


// SelectionWaveTest function should set:
// - last_ITCXOPError
// - last_ITCError
Function SelectionWaveTest_Prototype(selectionWave)
	WAVE selectionWave
End


/// validSelectionWave should:
/// - Contain valid values (no DLL errors)
/// - Have the minimum number of rows to be a valid selection wave
///   for function under test
///
Function testSelectionWave(testFn_1ch, testFn_2ch, validSelectionWave_1ch, validSelectionWave_2ch)
	FUNCREF SelectionWaveTest_Prototype testFn_1ch, testFn_2ch
	WAVE validSelectionWave_1ch, validSelectionWave_2ch

	variable error


	Variable NT_INCOMPATIBLE = 16
	Variable NEED_MIN_ROWS = 10019
	Variable EXPECT_MATRIX = 408

	//-------------------------------------------------------------
	// Test with valid supplied selection wave
	testValid(testFn_1ch, validSelectionWave_1ch)
	testValid(testFn_2ch, validSelectionWave_2ch)

	// Test both 1D and 2D waves work in the 1-channel case
	// 1D -- DimSize(wave, 1) = 0
	// 1D -- DimSize(wave, 1) = 1
	testValid(testFn_1ch, make_1ch_1D(validSelectionWave_1ch))
	testValid(testFn_1ch, make_1ch_2D(validSelectionWave_1ch))

	//-------------------------------------------------------------
	// Test longer selection waves work
	testValid_Long(testFn_1ch, validSelectionWave_1ch)
	testValid_Long(testFn_2ch, validSelectionWave_2ch)

	// Again, test 1D and 2D waves in the 1-channel case
	testValid_Long(testFn_1ch, make_1ch_1D(validSelectionWave_1ch))
	testValid_Long(testFn_1ch, make_1ch_2D(validSelectionWave_1ch))

	//-------------------------------------------------------------
	// Test with wrong type of wave
	testInvalid_wrongType(testFn_1ch, validSelectionWave_1ch, NT_INCOMPATIBLE)
	testInvalid_wrongType(testFn_2ch, validSelectionWave_2ch, NT_INCOMPATIBLE)

	testInvalid_wrongType(testFn_1ch, make_1ch_1D(validSelectionWave_1ch), NT_INCOMPATIBLE)
	testInvalid_wrongType(testFn_1ch, make_1ch_2D(validSelectionWave_1ch), NT_INCOMPATIBLE)


	//-------------------------------------------------------------
	// Test with wrong length of wave
	testInvalid_tooShort(testFn_1ch, validSelectionWave_1ch, NEED_MIN_ROWS)
	testInvalid_tooShort(testFn_2ch, validSelectionWave_2ch, NEED_MIN_ROWS)

	testInvalid_tooShort(testFn_1ch, make_1ch_1D(validSelectionWave_1ch), NEED_MIN_ROWS)
	testInvalid_tooShort(testFn_1ch, make_1ch_2D(validSelectionWave_1ch), NEED_MIN_ROWS)

	//-------------------------------------------------------------
	// Test with invalid 3D wave
	testInvalid_wrongDimensions(testFn_1ch, validSelectionWave_1ch, EXPECT_MATRIX)
	testInvalid_wrongDimensions(testFn_2ch, validSelectionWave_2ch, EXPECT_MATRIX)
End

Static Function /WAVE make_1ch_1D(validSelectionWave_1ch)
	WAVE validSelectionWave_1ch

	Duplicate /FREE validSelectionWave_1ch, validSelectionWave1D
	Redimension /N=(-1,0) validSelectionWave1D

	return validSelectionWave1D
End

Static Function /WAVE make_1ch_2D(validSelectionWave_1ch)
	WAVE validSelectionWave_1ch

	Duplicate /FREE validSelectionWave_1ch, validSelectionWave1D
	Redimension /N=(-1,1) validSelectionWave1D

	return validSelectionWave1D
End

Static Function testValid(testFn, validSelectionWave)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE validSelectionWave

	testFn(validSelectionWave)

	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, 0)
	CHECK_EQUAL_VAR(last_ITCError, 0)

End

Static Function testValid_Long(testFn, validSelectionWave)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE validSelectionWave

	Duplicate /FREE validSelectionWave, validSelectionWave_Long
	Redimension /N=(DimSize(validSelectionWave,0)*2 + 1, -1) validSelectionWave_Long

	testValid(testFn, validSelectionWave_Long)
End

Static Function testInvalid(testFn, invalidSelectionWave, expected_ITCXOPError)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE invalidSelectionWave
	Variable expected_ITCXOPError

	variable error

	Try
		testFn(invalidSelectionWave); AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry


	NVAR last_ITCXOPError, last_ITCError
	CHECK_EQUAL_VAR(last_ITCXOPError, expected_ITCXOPError)
	CHECK_EQUAL_VAR(last_ITCError, 0)
End


Static Function testInvalid_wrongType(testFn, validSelectionWave, expected_ITCXOPError)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE validSelectionWave
	Variable expected_ITCXOPError

	Duplicate /FREE validSelectionWave, invalidSelectionWave
	Redimension /C/D invalidSelectionWave

	testInvalid(testFn, invalidSelectionWave, expected_ITCXOPError)
End

Static Function testInvalid_tooShort(testFn, validSelectionWave, expected_ITCXOPError)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE validSelectionWave
	Variable expected_ITCXOPError

	Duplicate /FREE validSelectionWave, invalidSelectionWave
	Redimension /N=(DimSize(validSelectionWave, 0) - 2, -1) invalidSelectionWave

	testInvalid(testFn, invalidSelectionWave, expected_ITCXOPError)
End

Static Function testInvalid_wrongDimensions(testFn, validSelectionWave, expected_ITCXOPError)
	FUNCREF SelectionWaveTest_Prototype testFn
	WAVE validSelectionWave
	Variable expected_ITCXOPError

	// Make sure that the size of 2nd dimension is going to be at least 1
	// If it's 0, then we end up with a 1D wave
	Variable dimSize1 = min(1, DimSize(validSelectionWave, 1))

	Duplicate /FREE validSelectionWave, invalidSelectionWave
	Redimension /N=(DimSize(validSelectionWave, 0), dimSize1, 1) invalidSelectionWave

	testInvalid(testFn, invalidSelectionWave, expected_ITCXOPError)
End
