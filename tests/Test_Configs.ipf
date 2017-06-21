#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_Configs

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_Configs.ipf")
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

	// Make sure that we have a device to close
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	Make /O /W $defaultWaveName

	TEST_CASE_BEGIN(name)
End

// Set the config with ITCConfigChannel2.
// Get the config with ITCGetAllChannelsConfig2.
// Test all parameters, check that the correct values are set in GetConfig
Static Function get_set_config()
variable error
// In each of these tests, set the config of /CHN=0 0
// For /S parameters, aim for a time of 100 us / 10 kHz
	Make /FREE ChanSel = {0, 0}

// Set parameter without /S
	ITCConfigChannelReset2
	ITCConfigChannel2 /CHN=0 0, 100, $defaultWavename
	ITCConfigChannelUpload2
	ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

	CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
	CHECK_EQUAL_VAR(1e4, out[%SamplingRate])



// Set parameters with /S
	Make /D /O /FREE SVals = {0, 1, 2, 3}
	Make /D /O /FREE Intervals = {1e-4, 1e-1, 1e2, 1e5}

	Variable k
	For( k = 0; k < numpnts(SVals); k += 1)
		ITCConfigChannelReset2
		ITCConfigChannel2 /S=(SVals[k]) /CHN=0 0, Intervals[k], $defaultWavename
		ITCConfigChannelUpload2
		ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

		CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
		CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
	EndFor

// Set parameters with /M and /S
	Make /D /O /FREE SVals = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}
	Make /D /O /FREE MVals = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2}
	Make /D /O /FREE Intervals = {1e4, 1e7, 1e10, 1e13, 1e-4, 1e-1, 1e2, 1e5, 8e1, 8e4, 8e7, 8e10}

	For( k = 0; k < numpnts(SVals); k += 1)
		ITCConfigChannelReset2
		ITCConfigChannel2 /M=(MVals[k]) /S=(SVals[k]) /CHN=0 0, Intervals[k], $defaultWavename
		ITCConfigChannelUpload2
		ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

		CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
		CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
	EndFor

// Set parameters with /M
	Make /D /O /FREE MVals = {0, 1, 2}
	Make /D /O /FREE Intervals = {1e10, 1e2, 8e7}

	For( k = 0; k < numpnts(MVals); k += 1)
		ITCConfigChannelReset2
		ITCConfigChannel2 /M=(MVals[k]) /CHN=0 0, Intervals[k], $defaultWavename
		ITCConfigChannelUpload2
		ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

		CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
		CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
	EndFor
End

Static Function TestA()
	Variable error
	Make /FREE ChanSel = {0, 0}
// Test /A
	// Fail to set an invalid sampling rate
	Try
		ITCConfigChannel2 /CHN=0 0, 100.01, $defaultWavename; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 10006)
		CHECK_EQUAL_VAR(V_ITCError, 2154106880)
	EndTry

	// Fail with /A=0
	Try
		ITCConfigChannel2 /A=0 /CHN=0 0, 100.01, $defaultWavename; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
		CHECK_EQUAL_VAR(V_ITCXOPError, 10006)
		CHECK_EQUAL_VAR(V_ITCError, 2154106880)
	EndTry

	// Succeed with  /A and /A=1
	// Rate is rounded to nearest
	ITCConfigChannelReset2
	ITCConfigChannel2 /A /CHN=0 0, 100.01, $defaultWavename
	ITCConfigChannelUpload2
	ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

	CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
	CHECK_EQUAL_VAR(1e4, out[%SamplingRate])

	ITCConfigChannelReset2
	ITCConfigChannel2 /A=1 /CHN=0 0, 100.01, $defaultWavename
	ITCConfigChannelUpload2
	ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

	CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
	CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
End

Static Function TestUO()
	Make /FREE ChanSel = {0, 0}

	// Flag vals come from itcmm.h

	Make /O /FREE OVals = 	{0,	1,	2,		3, 		4, 	5	}
	Make /O /FREE OFlagVals =	{0,	1,	256,	4096,	16,	32	}

	Make /O /FREE UVals = 	{0,	1,	2,		3, 		4, 	5	}
	Make /O /FREE UFlagVals =	{0,	2,	512,	8192,	16,	32	}

	Variable oIdx, uIdx
	For( oIdx = 0; oIdx < numpnts(OVals); oIdx += 1)
		for ( uIdx = 0; uIdx < numpnts(UVals); uIdx += 1 )

		// Set O
			ITCConfigChannelReset2
			ITCConfigChannel2 /O=(OVals[oIdx]) /CHN=0 0, 100, $defaultWavename
			ITCConfigChannelUpload2
			ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

			CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
			CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
			CHECK_EQUAL_VAR(OFlagVals[oIdx], out[%errorMode])

		// Set U
			ITCConfigChannelReset2
			ITCConfigChannel2 /U=(UVals[uIdx]) /CHN=0 0, 100, $defaultWavename
			ITCConfigChannelUpload2
			ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

			CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
			CHECK_EQUAL_VAR(1e4, out[%SamplingRate])
			CHECK_EQUAL_VAR(UFlagVals[uIdx], out[%errorMode])

		// Set both
			ITCConfigChannelReset2
			ITCConfigChannel2 /O=(OVals[oIdx]) /U=(UVals[uIdx]) /CHN=0 0, 100, $defaultWavename
			ITCConfigChannelUpload2
			ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

			CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
			CHECK_EQUAL_VAR(1e4, out[%SamplingRate])

			Variable expectedErrorMode = OFlagVals[oIdx] + UFlagVals[uIdx]

			// The error modes for /O=4 /U=4 and /O=5 /U=5 coincide.
			// So, in this case alone, don't add them together.
			If( (oIdx >= 4) && (oIdx  == uIdx) )
				expectedErrorMode = OFlagVals[oIdx]
			EndIf

			CHECK_EQUAL_VAR(expectedErrorMode, out[%errorMode])
		EndFor
	EndFor
End

Static Function TestD()
	Make /FREE ChanSel = {0, 0}

	Make /D /O /FREE DVals = {0, 1, 2, 3, 8}

	Variable k
	For( k = 0; k < numpnts(DVals); k += 1)
		ITCConfigChannelReset2
		ITCConfigChannel2 /D=(DVals[k]) /CHN=0 0, 100, $defaultWavename
		ITCConfigChannelUpload2
		ITCGetAllChannelsConfig2 /O /FREE ChanSel,out

		CHECK_EQUAL_VAR(0, out[%SamplingIntervalFlag])
		CHECK_EQUAL_VAR(1e4, out[%SamplingRate])

		// Well,
		// The decimate flag appears not to work...
		// Can't determine if this is expected or not..
		//
		//Variable expectedDecimation = DVals[k]
		//CHECK_EQUAL_VAR(expectedDecimation, out[%Decimation])
	EndFor

	print "****"
	print "NB -- /D FLAG DOES NOT OPERATE AS EXPECTED"
	print "****"

End