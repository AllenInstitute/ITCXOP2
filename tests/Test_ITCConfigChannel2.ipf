#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ConfigChannel2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCConfigChannel2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultCHN = 0
Static StrConstant		defaultCHS = "IN"
Static Constant		defaultChannel = 0
Static Constant		defaultSamplingInterval = 100
Static StrConstant		defaultWaveName = "root:fifoWAVE"

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// Make sure that we have a device to config
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	Make /O /W $defaultWaveName

	TEST_CASE_BEGIN(name)
End

// Test the most basic form:
// ITCConfigChannel2/DEV=number:deviceID/CHN=number:channelTypeNumeric/CHS=string:channelTypeString number:channelNumber, number:samplingInterval, wave:data

/// Check all valid combinations of CHN and channelNumber do not produce an error
/// NB -- assuming ITC18USB

Static Function configChannel_chn_fn_IGNORE(chn, channelNumber)
	variable chn, channelNumber

	ITCConfigChannel2 /CHN=(chn) channelNumber, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannel_chs_fn_IGNORE(chs, channelNumber)
	string chs
	variable channelNumber

	ITCConfigChannel2 /CHS=chs channelNumber, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannel_chn_chs_fn_IGNORE(chn, chs, channelNumber)
	variable chn
	string chs
	variable channelNumber

	ITCConfigChannel2 /CHN=(chn)/CHS=chs channelNumber, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannel_none_fn_IGNORE(channelNumber)
	variable channelNumber

	ITCConfigChannel2 channelNumber, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

//Static Function configChannel_test_chn_chs()
//	FUNCREF chn_fn_proto chn_fn = configChannel_chn_fn_IGNORE
//	FUNCREF chs_fn_proto chs_fn = configChannel_chs_fn_IGNORE
//	FUNCREF chn_chs_fn_proto chn_chs_fn = configChannel_chn_chs_fn_IGNORE
//	FUNCREF none_fn_proto none_fn = configChannel_none_fn_IGNORE
//
//	// PROBLEM
//	//
//	// When:
//	//   /CHN=4, channelNumber = 0
//	//   /CHS="AI", channelNumber = 0
//	// The DLL raises an error:
//	// Direction (0x80631000)
//	//
//	// Interestingly, this error is NOT raised when:
//	//   /CHN=0, channelNumber = 9
//	//   /CHS="IN", channelNumber = 9
//	//	despite the fact that this refers to the same channel...
//	//
//	//
//	// At present, skipping testing /CHN=4...
//
//	Variable BAD_CHANNEL_NUMBER_DLL_ERROR = 2154041345
//	Variable SKIP_CHANNEL = 4
//	chn_chs_test(chn_fn, chs_fn, chn_chs_fn, none_fn, SKIP_CHANNEL, BAD_CHANNEL_NUMBER_DLL_ERROR)
//End

Static Function configChannel_devID_IGNORE(devID)
	variable devID
	ITCConfigChannel2 /DEV=(devID) /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannel_noDevID_IGNORE()
	ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function configChannel_test_devID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = configChannel_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = configChannel_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

// Test the case of no extra flags set

Static Function noFlagFn_IGNORE()
	ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function noFlagTest()
	TestNoFlags(noFlagFn_IGNORE)
End


// Test the various flags for error conditions.
// The test for proper operation of the flag requires the use of ITCGetAllChannelsConfig2.
// These are tested together elsewhere.

Static Function testS()
	Variable error

	Make /D FlagVals				= {0, 		1,		2, 		3, 		4,		inf,	NaN,	-1	}
	Make /D SamplingIntervalVals	= {1e-4,	1e-1,	1e2,	1e5,	1e2,	1e2,	1e2,	1e2 }
	Make /D expectedITCXOPErrors	= {0,		0,		0,		0,		10014,	863,	863,	823	}
	Make /D expectedITCErrors		= {0,		0,		0,		0,		0,		0,		0,		0	}

	Variable currVal
	For( currVal = 0; currVal < numpnts(FlagVals); currVal += 1 )
		Variable currFlagVal = FlagVals[currVal]
		Variable currSamplingInterval = SamplingIntervalVals[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]

		Try
			ITCConfigChannel2 /S=(currFlagVal) /CHN=(defaultCHN) defaultChannel, currSamplingInterval, $defaultWaveName; AbortOnRTE
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

Static Function testM()
	Variable error

	// /M=0 -- Sampling rate set as frequency
	// /S multiplier is applied to frequency
	//
	// Without /M=0 flag:
	// -> /S flag means "milliseconds" -- period is divided by 1000
	// -> Sampling interval set to 100s / 1000 = 100ms
	// -> Sampling rate is 10 Hz
	//
	// With /M=0 flag
	// -> /S flag now means divide RATE by 1000
	// ->
	// -> Sampling rate set to 100 Hz / 1000 = 0.1 Hz

	// /M=2 -- Sampling rate set as ticks
	// 8e5 ticks per second on ITC18USB
	// /S multiplier applied to ticks
	//
	// So:
	// /M=2 /S=0, sampleInterval of 8e5  ticks -> 1 s
	// /M=2 /S=1, sampleInterval of 8e8  milli-ticks -> 1 s
	// /M=2 /S=2, sampleInterval of 8e11 micro-ticks -> 1 s
	//
	// According to the above, all of the SamplingIntervalVals should set the rate to 10 kHz.
	Make /D FlagVals				= {0, 		1,		2, 		3, 		inf,	NaN,	-1	}
	Make /D SamplingIntervalVals	= {1e10,	1e2,	8e7,	1e2,	1e2,	1e2,	1e2 }
	Make /D expectedITCXOPErrors	= {0,		0,		0,		10015,	863,	863,	823	}
	Make /D expectedITCErrors		= {0,		0,		0,		0,		0,		0,		0	}

	Variable currVal
	For( currVal = 0; currVal < numpnts(FlagVals); currVal += 1 )
		Variable currFlagVal = FlagVals[currVal]
		Variable currSamplingInterval = SamplingIntervalVals[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]

		Try
			ITCConfigChannel2 /M=(currFlagVal) /CHN=(defaultCHN) defaultChannel, currSamplingInterval, $defaultWaveName; AbortOnRTE
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

Static Function AflagValueFn_IGNORE(val)
	Variable val

	ITCConfigChannel2 /A=(val) /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testA()
	Variable expectedITCXOPError, expectedITCError

	// Test all valid values
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(AflagValueFn_IGNORE, 0, expectedITCXOPError, expectedITCError)
	TestValue(AflagValueFn_IGNORE, 1, expectedITCXOPError, expectedITCError)

	// Test smallest positive invalid value
	expectedITCXOPError = 10016
	expectedITCError = 0
	TestValue(AflagValueFn_IGNORE, 2, expectedITCXOPError, expectedITCError)

	// Test -1
	expectedITCXOPError = 823
	expectedITCError = 0
	TestValue(AflagValueFn_IGNORE, -1, expectedITCXOPError, expectedITCError)

	// Test NaN/Inf
	TestNaNInf(AflagValueFn_IGNORE)
End

Static Function OflagValueFn_IGNORE(val)
	Variable val

	ITCConfigChannel2 /O=(val) /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testO()
	Variable expectedITCXOPError, expectedITCError

	// Test all valid values
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(OflagValueFn_IGNORE, 0, expectedITCXOPError, expectedITCError)
	TestValue(OflagValueFn_IGNORE, 1, expectedITCXOPError, expectedITCError)
	TestValue(OflagValueFn_IGNORE, 2, expectedITCXOPError, expectedITCError)
	TestValue(OflagValueFn_IGNORE, 3, expectedITCXOPError, expectedITCError)
	TestValue(OflagValueFn_IGNORE, 4, expectedITCXOPError, expectedITCError)
	TestValue(OflagValueFn_IGNORE, 5, expectedITCXOPError, expectedITCError)

	// Test smallest positive invalid value
	expectedITCXOPError = 10017
	expectedITCError = 0
	TestValue(OflagValueFn_IGNORE, 6, expectedITCXOPError, expectedITCError)

	// Test -1
	expectedITCXOPError = 823
	expectedITCError = 0
	TestValue(OflagValueFn_IGNORE, -1, expectedITCXOPError, expectedITCError)

	// Test NaN/Inf
	TestNaNInf(OflagValueFn_IGNORE)
End


Static Function UflagValueFn_IGNORE(val)
	Variable val

	ITCConfigChannel2 /U=(val) /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testU()
	Variable expectedITCXOPError, expectedITCError

	// Test all valid values
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(UflagValueFn_IGNORE, 0, expectedITCXOPError, expectedITCError)
	TestValue(UflagValueFn_IGNORE, 1, expectedITCXOPError, expectedITCError)
	TestValue(UflagValueFn_IGNORE, 2, expectedITCXOPError, expectedITCError)
	TestValue(UflagValueFn_IGNORE, 3, expectedITCXOPError, expectedITCError)
	TestValue(UflagValueFn_IGNORE, 4, expectedITCXOPError, expectedITCError)
	TestValue(UflagValueFn_IGNORE, 5, expectedITCXOPError, expectedITCError)

	// Test smallest positive invalid value
	expectedITCXOPError = 10018
	expectedITCError = 0
	TestValue(UflagValueFn_IGNORE, 6, expectedITCXOPError, expectedITCError)

	// Test -1
	expectedITCXOPError = 823
	expectedITCError = 0
	TestValue(UflagValueFn_IGNORE, -1, expectedITCXOPError, expectedITCError)

	// Test NaN/Inf
	TestNaNInf(UflagValueFn_IGNORE)
End

Static Function DflagValueFn_IGNORE(val)
	Variable val

	ITCConfigChannel2 /D=(val) /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, $defaultWaveName

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testD()
	Variable negativeOneAllowed = 0
	TestDWORDValueRange(DflagValueFn_IGNORE, negativeOneAllowed)


	Variable expectedITCXOPError, expectedITCError

	// Test some valid values
	expectedITCXOPError = 0
	expectedITCError = 0
	TestValue(DflagValueFn_IGNORE, 0, expectedITCXOPError, expectedITCError)
	TestValue(DflagValueFn_IGNORE, 1, expectedITCXOPError, expectedITCError)
	TestValue(DflagValueFn_IGNORE, 2, expectedITCXOPError, expectedITCError)
	TestValue(DflagValueFn_IGNORE, 3, expectedITCXOPError, expectedITCError)
End

// Expecting DLL errors for invalid sampling intervals
// Usually 2154106880 -- "Sampling Rate"
// Sampling interval of -1 gives:
// 2154172417 -- "Start Offset"
Static Function testSamplingInterval()
	Variable error

	Make /D SamplingIntervalVals	= {100, 	1e-3,			1e10, 			inf,			NaN,			-1			}
	Make /D expectedITCXOPErrors	= {0,		0,			0,			0,			0,			0		}
	Make /D expectedITCErrors		= {0,		2154106880,	2154106880,	2154106880,	2154106880,	2154172417	}

	Variable currVal
	For( currVal = 0; currVal < numpnts(SamplingIntervalVals); currVal += 1 )
		Variable currSamplingInterval = SamplingIntervalVals[currVal]
		Variable expectedITCError = expectedITCErrors[currVal]
		Variable expectedITCXOPError = expectedITCXOPErrors[currVal]

		Try
			ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, currSamplingInterval, $defaultWaveName; AbortOnRTE
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

Static Function testFIFO()
	Variable error

	// TODO:
	// So, in the XOP, we call HoldWave() to increment the reference count of the wave.
	// We ought to call ReleaseWave() to decrement the reference count once we're done with it.
	// Should we also check that input FIFOs aren't re-used???
	//
	// Reference counting should mean that free waves are retained (actually, indefinitely...)


	// Try incorrect wave type
	// Expecting error 16 "Incompatible Number Types"
	Make /D /FREE dblWave
	Try
		ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, dblWave; AbortOnRTE
		Fail()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 16)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry

	// Try empty wave reference
	// Expecting error 330 "Null wave op"
	WAVE /Z w
	Try
		ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, w; AbortOnRTE
		Fail()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 330)
		CHECK_EQUAL_VAR(V_ITCError, 0)
	EndTry


	// Try free wave
	Make /W /FREE freeWave
	ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, freeWave; AbortOnRTE
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	Make /O /W normalWave
	// Try normal wave
	ITCConfigChannel2 /CHN=(defaultCHN) defaultChannel, defaultSamplingInterval, normalWave
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)

End
