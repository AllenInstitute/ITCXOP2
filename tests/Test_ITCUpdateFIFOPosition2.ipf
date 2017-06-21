#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCUpdateFIFOPosition2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCUpdateFIFOPosition2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultCHN = 0
Static StrConstant	defaultCHS = "IN"
Static Constant		defaultChannel = 0
Static Constant		defaultValue = 0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// No devices open, so we can (re-)generate the config and FIFO waves
	Variable bufferLen = 65536


	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	ITCGetAllChannelsConfig2 /FREE *, allChannelsSelWave
	ITCCloseDevice2

	Variable totalNumChannels = DimSize(allChannelsSelWave, 1)

	Duplicate /O MakeConfigAllWave(totalNumChannels) root:globalConfWave
	Duplicate /O MakeFIFOWave(bufferLen,totalNumChannels) root:globalFIFOWave

	WAVE confWave = root:globalConfWave
	WAVE fifoWave = root:globalFIFOWave

	confWave[%ChannelType][] = allChannelsSelWave[%ChannelType][q]
	confWave[%ChannelNumber][] = allChannelsSelWave[%ChannelNumber][q]
	confWave[%SamplingInterval][] = 100

	// Set the output to a sine wave
	// 3200 -- V to DAC units
	// 10 -- 10 V range
	fifoWave[][] = 3200 * 10 * sin(2 * pi * (bufferLen/10) * p)

	// Make sure that we have a device open
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	// Upload the config
	// (Make sure we have FIFO waves set for all channels)
	ITCConfigAllChannels2 confWave, fifoWave

	TEST_CASE_BEGIN(name)
End


Static Function updateFIFOPos_devID_IGNORE(devID)
	variable devID
	ITCUpdateFIFOPosition2 /DEV=(devID) /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function updateFIFOPos_noDevID_IGNORE()
	ITCUpdateFIFOPosition2 /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function configChannel_test_devID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = updateFIFOPos_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = updateFIFOPos_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

Static Function updateFIFOPos_chn_fn_IGNORE(chn, channelNumber)
	variable chn, channelNumber

	ITCUpdateFIFOPosition2 /CHN=(chn) channelNumber, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function updateFIFOPos_chs_fn_IGNORE(chs, channelNumber)
	string chs
	variable channelNumber

	ITCUpdateFIFOPosition2 /CHS=chs channelNumber, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function updateFIFOPos_chn_chs_fn_IGNORE(chn, chs, channelNumber)
	variable chn
	string chs
	variable channelNumber

	ITCUpdateFIFOPosition2 /CHN=(chn)/CHS=chs channelNumber, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function updateFIFOPos_none_fn_IGNORE(channelNumber)
	variable channelNumber

	ITCUpdateFIFOPosition2 channelNumber, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function configChannel_test_chn_chs()
	FUNCREF chn_fn_proto chn_fn = updateFIFOPos_chn_fn_IGNORE
	FUNCREF chs_fn_proto chs_fn = updateFIFOPos_chs_fn_IGNORE
	FUNCREF chn_chs_fn_proto chn_chs_fn = updateFIFOPos_chn_chs_fn_IGNORE
	FUNCREF none_fn_proto none_fn = updateFIFOPos_none_fn_IGNORE

	Variable BAD_CHANNEL_NUMBER_DLL_ERROR = 0
	Variable SKIP_CHANNEL = NaN

	chn_chs_test(chn_fn, chs_fn, chn_chs_fn, none_fn, SKIP_CHANNEL, BAD_CHANNEL_NUMBER_DLL_ERROR)
End

// Test the various flags for error conditions.
// The test for proper operation of the flag requires getting the FIFO positions.
// These will be tested together elsewhere.

Static Function noFlagFn_IGNORE()
	// No flag
	ITCUpdateFIFOPosition2 /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function RflagOnlyFn_IGNORE()
	// Flag only
	ITCUpdateFIFOPosition2 /R /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function RflagValueFn_IGNORE(value)
	Variable value

	// Flag and value
	ITCUpdateFIFOPosition2 /R=(value) /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function LflagOnlyFn_IGNORE()
	// Flag only
	ITCUpdateFIFOPosition2 /L /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function LflagValueFn_IGNORE(value)
	Variable value

	// Flag and value
	ITCUpdateFIFOPosition2 /L=(value) /CHN=(defaultCHN) defaultChannel, defaultValue

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testValueFn_IGNORE(value)
	Variable value

	ITCUpdateFIFOPosition2 /CHN=(defaultCHN) defaultChannel, value

	Variable /G last_ITCXOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Function testParameters()
	variable negativeOneAllowed = 0

	TestNoFlags(noFlagFn_IGNORE)

	TestBooleanFlag(RflagOnlyFn_IGNORE, RflagValueFn_IGNORE, negativeOneAllowed)
	TestBooleanFlag(LflagOnlyFn_IGNORE, LflagValueFn_IGNORE, negativeOneAllowed)

	// -1 allowed for value
	negativeOneAllowed = 1
	TestDWORDValueRange(testValueFn_IGNORE, negativeOneAllowed)

	// There doesn't seem to be any kind of test in the DLL
	// that the value passed to ITCUpdateFIFOPosition
	// is in range.
	// Don't test invalid values that we know don't throw an error.

	return 0

	Variable DWORD_MAX = 4294967295
	Variable testVal = DWORD_MAX - 5
	Variable ITC_DLL_ERROR = 10006
	Variable expectedITCXOPError = ITC_DLL_ERROR
	Variable expectedITCError = 0	 // TODO -- doesn't produce an error code

	TestValue(testValueFn_IGNORE, testVal, expectedITCXOPError, expectedITCError)
End
