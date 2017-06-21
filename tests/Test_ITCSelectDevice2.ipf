#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCSelectDevice2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCSelectDevice2.ipf")
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

Static Function Test_ITCSelectDevice2()
	variable error

	Make /D /FREE deviceNums		= {0, 1, 		-1,	NaN,	inf,	-inf	}
	Make /D expectedITCXOPErrors	= {0, 10004,	823,	863,	863,	863	}
	Make /D expectedITCErrors		= {0, 0, 		0,		0,		0,		0		}

	Variable currVal
	For( currVal = 0; currVal < numpnts(deviceNums); currVal += 1 )
		Variable currDeviceNum = deviceNums[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]

		Try
			ITCSelectDevice2 currDeviceNum; AbortOnRTE
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

	EndFor
End
