#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCUpdateFIFOPositionAll2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCUpdateFIFOPositionAll2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

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

// Test the /DEV flag

Static Function updateFifoAll_devID_IGNORE(devID)
	variable devID

	Wave fifoPosWave = makeFIFOPositionWave(1)

	ITCUpdateFIFOPositionAll2 /DEV=(devID) fifoPosWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function updateFifoAll_noDevID_IGNORE()

	Wave fifoPosWave = makeFIFOPositionWave(1)

	ITCUpdateFIFOPositionAll2 fifoPosWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = updateFifoAll_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = updateFifoAll_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End


// Test the config wave
Static Function testFn_1ch(fifoPosWave)
	WAVE fifoPosWave

	ITCUpdateFIFOPositionAll2 fifoPosWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function testFn_2ch(fifoPosWave)
	WAVE fifoPosWave

	ITCUpdateFIFOPositionAll2 fifoPosWave

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError
End

Static Function test_confWave()
	Wave fifoPosWave1ch = makeFIFOPositionWave(1)
	Wave fifoPosWave2ch = makeFIFOPositionWave(2)
	fifoPosWave2ch[1][1] = 1

	testSelectionWave(testFn_1ch, testFn_2ch, fifoPosWave1ch, fifoPosWave2ch)
End

// Test with * selection / out wave
Static Function test_null()
	variable error

	// Using * as the selection wave should give NULL_WAVE_OP
	Variable NULL_WAVE_OP = 330

	Try
		ITCUpdateFIFOPositionAll2 *; AbortOnRTE
		FAIL()
	Catch
		error = GetRTError(1)
	EndTry

	CHECK_EQUAL_VAR(V_ITCXOPError, NULL_WAVE_OP)
	CHECK_EQUAL_VAR(V_ITCError, 0)
End
