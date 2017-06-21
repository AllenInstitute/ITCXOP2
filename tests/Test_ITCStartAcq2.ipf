#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCStartAcq2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCStartAcq2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     =  0

Static Constant		defaultChannel = 0
Static Constant		numDACChannels = 4
Static Constant		defaultVoltage =0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	// Close any devices left open
	ITCCloseAll2

	// No devices open, so we can (re-)generate the config and FIFO waves
	Variable bufferLen = 65536

	Duplicate /O MakeConfigAllWave(2) root:globalConfWave
	Duplicate /O MakeFIFOWave(bufferLen,2) root:globalFIFOWave

	WAVE confWave = root:globalConfWave
	WAVE fifoWave = root:globalFIFOWave

	confWave[%ChannelType][1] = 1
	confWave[%SamplingInterval][] = 100

	// Set the output to a sine wave
	// 3200 -- V to DAC units
	// 10 -- 10 V range
	fifoWave[][1] = 3200 * 10 * sin(2 * pi * (bufferLen/10) * p)

	// Make sure that we have a device to setDAC on
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	Variable /G $lastDevIDVariableName = V_Value

	TEST_CASE_BEGIN(name)
End

// Assumes that the config and FIFO waves have been set up properly
Static Function DoConfig_IGNORE()
	WAVE confWave = root:globalConfWave
	WAVE fifoWave = root:globalFIFOWave

	// ITCConfigAllChannels2:
	// - Resets the config
	// - Sets the config
	// - Uploads the config
	ITCConfigAllChannels2 confWave, fifoWave
End

// Test the /DEV flag

Static Function startAcq_devID_IGNORE(devID)
	variable devID

	DoConfig_IGNORE()
	ITCStartAcq2 /DEV=(devID)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function startAcq_noDevID_IGNORE()

	DoConfig_IGNORE()
	ITCStartAcq2

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

static Function testDevID()
	FUNCREF DevIDTests_devID_Prototype devID_fn = startAcq_devID_IGNORE
	FUNCREF DevIDTests_noDevID_Prototype noDevID_fn = startAcq_noDevID_IGNORE
	devIDTest(devID_fn, noDevID_fn)
End

Static Function noFlagFn_IGNORE()

	DoConfig_IGNORE()
	ITCStartAcq2

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function RflagOnlyFn_IGNORE()

	DoConfig_IGNORE()
	ITCStartAcq2 /R

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function RflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /R=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function FflagOnlyFn_IGNORE()

	DoConfig_IGNORE()
	ITCStartAcq2 /F

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function FflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /F=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function OEflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /OE=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function EXTflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /EXT=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function STPOflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /STPO=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function STPUflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /STPU=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function STRTflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /STRT=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End


Static Function STOPflagValueFn_IGNORE(value)
	Variable value

	DoConfig_IGNORE()
	ITCStartAcq2 /STOP=(value)

	Variable /G last_ITCxOPError = V_ITCXOPError
	Variable /G last_ITCError = V_ITCError

	ITCStopAcq2
End

Static Function testFlags()
	Variable negativeOneAllowed

	// Test the case of no flags
	TestNoFlags(noFlagFn_IGNORE)

	// Test R boolean flag
	negativeOneAllowed = 0
	TestBooleanFlag(RflagOnlyFn_IGNORE, RflagValueFn_IGNORE, negativeOneAllowed)

	// Test F flag
	negativeOneAllowed = 1
	TestFlagOnly(FflagOnlyFn_IGNORE)
	TestDWORDValueRange(FflagValueFn_IGNORE, negativeOneAllowed)

	// Test OE flag
	negativeOneAllowed = 1
	TestDWORDValueRange(OEflagValueFn_IGNORE, negativeOneAllowed)

	// Test EXT flag
	negativeOneAllowed = 1
	TestDWORDValueRange(EXTflagValueFn_IGNORE, negativeOneAllowed)

	// Test STOPO flag
	negativeOneAllowed = 1
	TestDWORDValueRange(STPOflagValueFn_IGNORE, negativeOneAllowed)

	// Test STOPU flag
	negativeOneAllowed = 1
	TestDWORDValueRange(STPUflagValueFn_IGNORE, negativeOneAllowed)

	// Test STARTT flag
	TestNanInf(STRTflagValueFn_IGNORE)

	// Test STOPT flag
	TestNanInf(STOPflagValueFn_IGNORE)
End
