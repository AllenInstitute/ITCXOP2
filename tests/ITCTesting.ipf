#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#include ":CheckDimLabels"

#include ":CHN_CHS_Tests"
#include ":DevIDTests"
#include ":FlagValuesTests"
#include ":GenerateDestTest"
#include ":SelectionWaveTests"
#include ":Test_Configs"
#include ":Test_ITCCloseAll2"
#include ":Test_ITCCloseDevice2"
#include ":Test_ITCConfigAllChannels2"
#include ":Test_ITCConfigChannel2"
#include ":Test_ITCConfigChannelReset2"
#include ":Test_ITCConfigChannelUpload2"
#include ":Test_ITCFIFOAvailable2"
#include ":Test_ITCFIFOAvailableAll2"
#include ":Test_ITCGetAllChannelsConfig2"
#include ":Test_ITCGetChannelConfig2"
#include ":Test_ITCGetDevices2"
#include ":Test_ITCGetState2"
#include ":Test_ITCInitialize2"
#include ":Test_ITCOpenDevice2"
#include ":Test_ITCReadADC2"
#include ":Test_ITCReadDigital2"
#include ":Test_ITCReadTimer2"
#include ":Test_ITCSelectDevice2"
#include ":Test_ITCSetDAC2"
#include ":Test_ITCSetGlobals2"
#include ":Test_ITCStartAcq2"
#include ":Test_ITCStopAcq2"
#include ":Test_ITCUpdateFIFOPosition2"
#include ":Test_ITCUpdateFIFOPositionAll2"
#include ":Test_ITCWriteDigital2"
#include ":Test_ITCGetVersions2"

Function Run()
	RunTest(WinList("Test_*.ipf", ";", "WIN:128"))
End
