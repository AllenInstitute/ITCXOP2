#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=CHN_CHS_Tests

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// Check all valid combinations of CHN and channelNumber do not produce an error

/// Check invalid values for CHN, invalid values for CHS do produce an error.


	// TODO -- check error priorities?
	// (If there are multiple errors, which error is raised?)
	//
	// - /DEV flag
	// - /CHN flag
	// - /CHS flag
	// - CHN/CHS combination
	// - Channel number


/// NB -- assuming ITC18USB

Function chn_fn_proto(chn, channelNumber)
	variable chn, channelNumber
End

function chs_fn_proto(chs, channelNumber)
	string chs
	variable channelNumber
End

function chn_chs_fn_proto(chn, chs, channelNumber)
	variable chn
	string chs
	variable channelNumber
End

function none_fn_proto(channelNumber)
	variable channelNumber
End

Function chn_chs_test(chn_fn, chs_fn, chn_chs_fn, none_fn, skipChannel, BadChannelNumberError)
	FUNCREF chn_fn_proto chn_fn
	FUNCREF chs_fn_proto chs_fn
	FUNCREF chn_chs_fn_proto chn_chs_fn
	FUNCREF none_fn_proto none_fn
	Variable skipChannel, BadChannelNumberError

	test_chn_chs(chn_fn, chs_fn, chn_chs_fn, skipChannel)

	test_chn_invalid(chn_fn)
	test_chs_invalid(chs_fn)
	test_chn_chs_invalid(chn_chs_fn)
	test_none_invalid(none_fn)

	test_channelNumber_invalid(chn_fn, chs_fn, chn_chs_fn, BadChannelNumberError)
End

static Function test_chn_chs(chn_fn, chs_fn, chn_chs_fn, skipChannel)
	FUNCREF chn_fn_proto chn_fn
	FUNCREF chs_fn_proto chs_fn
	FUNCREF chn_chs_fn_proto chn_chs_fn
	Variable skipChannel

	Variable error

	// Possible values of CHS
	Make /FREE /T CHS_Vals = {"IN", "OUT", "AD", "DA", "DI", "DO", "AI", "AO"}

	// CHN value corresponding to each CHS value
	Make /FREE CHN_Vals = {0, 1, 0, 1, 2, 3, 4, 5}

	// Number of channels for each CHS value
	Make /FREE /N=8 numChannels
	numChannels[0] = 10	// IN
	numChannels[1] = 8	// OUT
	numChannels[2] = 8	// AD
	numChannels[3] = 4	// DA
	numChannels[4] = 1	// DI
	numChannels[5] = 2	// DO
	numChannels[6] = 1	// AI
	numChannels[7] = 2	// AO


	Variable /G last_ITCXOPError, last_ITCError
	NVAR last_ITCXOPError, last_ITCError

	Variable currIdx, currChannelNumber
	For( currIdx = 0; currIdx < numpnts(CHS_Vals); currIdx += 1)
		For( currChannelNumber = 0; currChannelNumber < numChannels[currIdx]; currChannelNumber += 1)
			Variable currCHNVal = CHN_Vals[currIdx]
			String currCHSVal = CHS_Vals[currIdx]

			if( currCHNVal == skipChannel )
				continue
			endif

			Try
				chn_fn(currCHNVal, currChannelNumber); AbortOnRTE
				PASS()
			Catch
				error = GetRTError(1)
				print "---> Problem in chn function.  /CHN=" + num2str(currCHNVal) + ", channelNumber=" + num2str(currChannelNumber)
				print "---> ITCXOPError: " + num2istr(last_ITCXOPError)
				print "---> ITCError: " + num2istr(last_ITCError)
			EndTry

			CHECK_EQUAL_VAR(last_ITCXOPError, 0)
			CHECK_EQUAL_VAR(last_ITCError, 0)

			Try
				chs_fn(currCHSVal, currChannelNumber); AbortOnRTE
				PASS()
			Catch
				error = GetRTError(1)
				print "---> Problem in chs function.  /CHS=" + currCHSVal + ", channelNumber=" + num2str(currChannelNumber)
				print "---> ITCXOPError: " + num2istr(last_ITCXOPError)
				print "---> ITCError: " + num2istr(last_ITCError)
			EndTry

			CHECK_EQUAL_VAR(last_ITCXOPError, 0)
			CHECK_EQUAL_VAR(last_ITCError, 0)

			Try
				chn_chs_fn(currCHNVal, currCHSVal, currChannelNumber); AbortOnRTE
				PASS()
			Catch
				error = GetRTError(1)
				print "---> Problem in chn_chs function.  /CHN=" + num2str(currCHNVal) +", /CHS=" + currCHSVal + ", channelNumber=" + num2str(currChannelNumber)
				print "---> ITCXOPError: " + num2istr(last_ITCXOPError)
				print "---> ITCError: " + num2istr(last_ITCError)
			EndTry

			CHECK_EQUAL_VAR(last_ITCXOPError, 0)
			CHECK_EQUAL_VAR(last_ITCError, 0)
		EndFor
	EndFor

End

static Function test_chn_invalid(chn_fn)
	FUNCREF chn_fn_proto chn_fn

	Variable channelNumber = 0

	// Nan/Inf error = 863
	// Parameter is out of range = 823
	// Invalid CHN value = 10010
	Variable DWORD_MAX = 4294967295
	Make /FREE CHN_Vals 					= { NaN,	inf,	-inf,	-1,	DWORD_MAX+1,	6,			1000	}
	Make /FREE expectedITCXOPErrors	= { 863,	863,	863,	823,	823,				10010,	10010 }
	Variable expectedITCError = 0

	Variable currCHN_Idx
	For( currCHN_Idx=0; currCHN_Idx < numpnts(CHN_Vals); currCHN_Idx += 1)
		Variable currCHN = CHN_Vals[currCHN_Idx]
		Variable expectedITCXOPError = expectedITCXOPErrors[currCHN_Idx]

		testCHN(chn_fn, currCHN, channelNumber, expectedITCXOPError, expectedITCError)
	EndFor
End

static Function test_chs_invalid(chs_fn)
	FUNCREF chs_fn_proto chs_fn

	String invalidCHSValue = "TEST"

	// Invalid CHS value = 10011
	Variable expectedITCXOPError = 10011
	Variable expectedITCError = 0

	testCHS(chs_fn, invalidCHSValue, 0, expectedITCXOPError, expectedITCError)
End

// Check all invalid combinations of CHN and CHS
// Expecting CHN_CHS_DISAGREE
static Function test_chn_chs_invalid(chn_chs_fn)
	FUNCREF chn_chs_fn_proto chn_chs_fn

	Variable error

	// Possible values of CHS
	Make /FREE /T CHS_Vals = {"IN", "OUT", "AD", "DA", "DI", "DO", "AI", "AO"}

	// CHN value corresponding to each CHS value
	Make /FREE CHN_Vals = {0, 1, 0, 1, 2, 3, 4, 5}

	// Number of channels for each CHS value
	Make /FREE /N=8 numChannels
	numChannels[0] = 10	// IN
	numChannels[1] = 8	// OUT
	numChannels[2] = 8	// AD
	numChannels[3] = 4	// DA
	numChannels[4] = 1	// DI
	numChannels[5] = 2	// DO
	numChannels[6] = 1	// AI
	numChannels[7] = 2	// AO

	variable currCHN_Idx, currCHS_Idx, currChannelNumber
	For( currCHN_Idx = 0; currCHN_Idx < numpnts(CHN_Vals); currCHN_Idx += 1)
		For( currCHS_Idx = 0; currCHS_Idx < numpnts(CHS_Vals); currCHS_Idx += 1)

			// Skip valid CHN/CHS combinations
			If( CHN_Vals[currCHN_Idx] == CHN_Vals[currCHS_Idx] )
				continue
			EndIf

			// Make sure we get the error for all valid channel numbers
			For( currChannelNumber = 0; currChannelNumber < numChannels[currCHN_Idx]; currChannelNumber += 1)
				Variable expectedITCXOPError = 10012
				Variable expectedITCError = 0

				testCHN_CHS(chn_chs_fn, CHN_Vals[currCHN_Idx], CHS_Vals[currCHS_Idx], currChannelNumber, expectedITCXOPError, expectedITCError)
			EndFor


		EndFor
	EndFor
End


// Valid /CHN and /CHS, invalid channel number
static Function test_channelNumber_invalid(chn_fn, chs_fn, chn_chs_fn, BadChannelNumber_ITCError)
	FUNCREF chn_fn_proto chn_fn
	FUNCREF chs_fn_proto chs_fn
	FUNCREF chn_chs_fn_proto chn_chs_fn
	Variable BadChannelNumber_ITCError

	Variable validCHN = 0
	String validCHS = "IN"

	// Work out what errors we're expecting for bad channel numbers

	variable ITC_DLL_ERROR = 10006
	variable BadChannelNumber_ITCXOPError

	If( BadChannelNumber_ITCError != 0 )
		BadChannelNumber_ITCXOPError = ITC_DLL_ERROR
	Else
		BadChannelNumber_ITCXOPError = 0
	EndIf


	// kDoesNotSupportNaNorINF = 863
	// kParameterOutOfRange = 823

	Variable DWORD_MAX = 4294967295
	Make /FREE /D channelNumberVals 		= { NaN,	inf,	-inf,	-1,	DWORD_MAX+1,	32,									1000									}
	Make /FREE /D expectedITCXOPErrors	= { 863,	863,	863,	823,	823,				BadChannelNumber_ITCXOPError,	BadChannelNumber_ITCXOPError	}
	Make /FREE /D expectedITCErrors		= { 0,	0,		0,		0, 	0,					BadChannelNumber_ITCError,		BadChannelNumber_ITCError		}

	Variable k
	For( k=0; k < numpnts(channelNumberVals); k += 1)

		Variable currChannelNumber = channelNumberVals[k]
		Variable expectedITCXOPError = expectedITCXOPErrors[k]
		Variable expectedITCError = expectedITCErrors[k]

		If(expectedITCXOPError == 0 && expectedITCError == 0)
			// Don't deliberately pass invalid values that we know won't give an error...
			continue
		EndIf

		// valid CHN, invalid channel number
		testCHN(chn_fn, validCHN, currChannelNumber, expectedITCXOPError, expectedITCError)

		// valid CHS, invalid channelNumber
		testCHS(chs_fn, validCHs, currChannelNumber, expectedITCXOPError, expectedITCError)

		// valid CHN/CHS, invalid channel number
		testCHN_CHS(chn_chs_fn, validCHN, validCHS, currChannelNumber, expectedITCXOPError, expectedITCError)

	EndFor
End

// Expecting MUST_SPECIFY_CHN_OR_CHS
// error code 10013
static Function test_none_invalid(none_fn)
	FUNCREF none_fn_proto none_fn
	variable expectedITCXOPError, expectedITCError

	expectedITCXOPError = 10013
	expectedITCError = 0


	Variable currChannelNumber
	For( currChannelNumber=0; currChannelNumber < 10; currChannelNumber += 1)
		TestValue(none_fn, currChannelNumber, expectedITCXOPError, expectedITCError)
	EndFor

End

Static Function testCHN(chn_fn, chn_value, channelNumber, expectedITCXOPError, expectedITCError)
	FUNCREF chn_fn_proto chn_fn
	Variable chn_value, channelNumber, expectedITCXOPError, expectedITCError

	variable error

	Try
		chn_fn(chn_value, channelNumber); AbortOnRTE

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

Static Function testCHS(chs_fn, chs_value, channelNumber, expectedITCXOPError, expectedITCError)
	FUNCREF chs_fn_proto chs_fn
	String chs_value
	Variable channelNumber, expectedITCXOPError, expectedITCError

	variable error

	Try
		chs_fn(chs_value, channelNumber); AbortOnRTE

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

Static Function testCHN_CHS(chn_chs_fn, chn_value, chs_value, channelNumber, expectedITCXOPError, expectedITCError)
	FUNCREF chn_chs_fn_proto chn_chs_fn
	Variable chn_value
	String chs_value
	Variable channelNumber, expectedITCXOPError, expectedITCError

	variable error

	Try
		chn_chs_fn(chn_value, chs_value, channelNumber); AbortOnRTE

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
