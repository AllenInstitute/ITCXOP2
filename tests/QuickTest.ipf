#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

Function Run()
	RunTest(WinList("Test_*.ipf", ";", "WIN:128"))
End

threadsafe function QuickTest()
	// Close any devices left open
	ITCCloseAll2

	// No devices open, so we can (re-)generate the config and FIFO waves
	Variable bufferLen = 2^17

	Duplicate /O MakeConfigAllWave(2) root:globalConfWave
	Duplicate /O MakeFIFOWave(bufferLen,2) root:globalFIFOWave
	Duplicate /O makeFIFOPositionWave(2) root:globalFIFOPosWave

	WAVE confWave = root:globalConfWave
	WAVE fifoWave = root:globalFIFOWave
	WAVE fifoPosWave = root:globalFIFOPosWave

	confWave[%ChannelType][1] = 1
	confWave[%SamplingInterval][] = 100

	// Set the output to a sine wave
	// 3200 -- V to DAC units
	// 10 -- 10 V range
	fifoWave[][1] = 3200 * 10 * sin(2 * pi * (10 / bufferLen) * p)

	// Set the fifoPosWave
	fifoPosWave[%ChannelType][1] = 1
	fifoPosWave[%Value][] = -1

	// Make sure that we have a device open
	ITCOpenDevice2 0

	// Upload the config
	// (Make sure we have a FIFO wave set)
	ITCConfigAllChannels2 confWave, fifoWave

	// Reset the FIFOs
	ITCUpdateFIFOPositionAll2 fifoPosWave

	ITCGetAllChannelsConfig2 /O confWave, outWave

	// Start acquisition
	ITCStartAcq2 /Z; AbortOnRTE

	variable lastValue

	Do
		ITCFIFOAvailable2 /CHN=0 0
		if(lastValue != V_Value)
			lastValue = V_Value
			print lastValue
		endif
	While (V_Value < bufferLen)

	ITCStopAcq2

	ITCCloseAll2
end


Function DoStuff()

	variable tgID = ThreadGroupCreate(1)

	Threadstart tgID, 0, QuickTest()

	do
		if(ThreadGroupWait(tgID,500) == 0)
			break
		endif
	while(1)

End
