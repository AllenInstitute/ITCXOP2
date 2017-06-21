#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCGetDevices2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCGetDevices2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     = 0

Static Constant numDevices		= 1

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to readADC from
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

static Function testGetDevices()
	ITCGetDevices2
	CHECK_EQUAL_VAR(V_Value, numDevices)
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

End

static Function testDTN_DTS()
	Variable error

	// Valid values must come first
	Make 		DTN = {0, 			1, 		2, 			3, 		4, 			5, 			-1,			6,			-2,		inf,		-inf,		NaN	}
	Make /T	DTS = {"ITC16", 	"ITC18", "ITC1600",	"ITC00",	"ITC16USB",	"ITC18USB",	"ITCALL",	"BAD",	"TEST",	"",		"BAD",	"BAD"	}
	Make numDev  = {0, 			0,			0,				0,			0,				1,				1,				0,			0,			0,			0,			0		}
	Make DTN_ERR = {0,			0,			0,				0,			0,				0,				0,				10007,	823,		863,		863,		863	}


	Variable numValidVals = 7
	Variable DTS_ERROR = 10008
	Variable DTN_DTS_DISAGREE = 10009
	Variable Error_V_Value = -1


	Variable currDTNIdx, currDTSIdx

	For (currDTNIdx = 0; currDTNIdx < numPnts(DTN); currDTNIdx += 1)

		For (currDTSIdx = 0; currDTSIdx < numPnts(DTS); currDTSIdx += 1)

			// Only DTN
			Try
				ITCGetDevices2 /DTN=(DTN[currDTNIdx]);AbortOnRTE
				If( currDTNIdx >= numValidVals )
					// No error on an invalid value
					FAIL()
				EndIf

				CHECK_EQUAL_VAR(V_ITCXOPError, 0)
				CHECK_EQUAL_VAR(V_ITCError, 0)
				CHECK_EQUAL_VAR(V_Value, numDev[currDTNIdx])
			Catch
				error = GetRTError(1)
				If( currDTNIdx < numValidVals)
					// Error on a valid value
					FAIL()
				EndIf
				CHECK_EQUAL_VAR(V_ITCXOPError, DTN_ERR[currDTNIdx])
				CHECK_EQUAL_VAR(V_ITCError, 0)
				CHECK_EQUAL_VAR(V_Value, Error_V_Value)
			EndTry


			// Only DTS
			Try
				ITCGetDevices2 /DTS=(DTS[currDTSIdx]);AbortOnRTE
				If( currDTSIdx >= numValidVals )
					// No error on an invalid value
					FAIL()
				EndIf

				CHECK_EQUAL_VAR(V_ITCXOPError, 0)
				CHECK_EQUAL_VAR(V_ITCError, 0)
				CHECK_EQUAL_VAR(V_Value, numDev[currDTSIdx])
			Catch
				error = GetRTError(1)
				If( currDTSIdx < numValidVals)
					// Error on a valid value
					FAIL()
				EndIf
				CHECK_EQUAL_VAR(V_ITCXOPError, DTS_ERROR)
				CHECK_EQUAL_VAR(V_ITCError, 0)
				CHECK_EQUAL_VAR(V_Value, Error_V_Value)
			EndTry

			// Both valid /DTN and /DTS specified
			Try
				ITCGetDevices2 /DTN=(DTN[currDTNIdx])/DTS=(DTS[currDTSIdx]);AbortOnRTE
				If( currDTNIdx >= numValidVals || currDTSIdx >= numValidVals )
					// No error on an invalid value
					FAIL()
				EndIf

				// If DTN idx and DTS idx are not equal, fail
				If( currDTNIdx != currDTSIdx )
					printf "Problem.  DTNIdx = %d, DTSIdx = %d\n", currDTNIdx, currDTSIdx
					FAIL()
				EndIf

				CHECK_EQUAL_VAR(V_ITCXOPError, 0)
				CHECK_EQUAL_VAR(V_ITCError, 0)
				CHECK_EQUAL_VAR(V_Value, numDev[currDTNIdx])
			Catch
				error = GetRTError(1)

				CHECK_EQUAL_VAR(V_Value, Error_V_Value)

				If( currDTNIdx >= numValidVals )
					// Expect DTN error if DTN is invalid, regardless of DTS validity, or DTN/DTS mismatch
					CHECK_EQUAL_VAR(V_ITCXOPError, DTN_ERR[currDTNIdx])
					CHECK_EQUAL_VAR(V_ITCError, 0)
					Continue
				EndIf

				If( currDTSIdx >= numValidVals )
					// Expect DTS error if DTN is valid, and DTS is invalid, regardless of DTN/DTS mismatch
					CHECK_EQUAL_VAR(V_ITCXOPError, DTS_ERROR)
					CHECK_EQUAL_VAR(V_ITCError, 0)
					Continue
				EndIf

				// Have encountered an error when DTN and DTS both valid,
				// and both DTN and DTS refer to the same device
				If( currDTNIdx == currDTSIdx )
					FAIL()
				EndIf

				// Check we got the correct error code back for a mismatch
				CHECK_EQUAL_VAR(V_ITCXOPError, DTN_DTS_DISAGREE)
				CHECK_EQUAL_VAR(V_ITCError, 0)
			EndTry

		EndFor

	EndFor

End
