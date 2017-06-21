#include "RegisterOperations.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

namespace
{

int RegisterITCCloseAll2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCCloseAll2RuntimeParams structure as well.
  cmdTemplate       = "ITCCloseAll2/Z[=number:displayErrors]";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCCloseAll2RuntimeParams),
                           (void *) ExecuteITCCloseAll2,
                           kOperationIsThreadSafe);
}

int RegisterITCCloseDevice2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCCloseDevice2RuntimeParams structure as well.
  cmdTemplate = "ITCCloseDevice2/Z[=number:displayErrors]/DEV=number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCCloseDevice2RuntimeParams),
                           (void *) ExecuteITCCloseDevice2,
                           kOperationIsThreadSafe);
}

int RegisterITCConfigAllChannels2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCConfigAllChannels2RuntimeParams structure as well.
  cmdTemplate = "ITCConfigAllChannels2/Z[=number:displayErrors]/"
                "DEV=number:deviceID wave:config, wave:data";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCConfigAllChannels2RuntimeParams),
                           (void *) ExecuteITCConfigAllChannels2,
                           kOperationIsThreadSafe);
}

int RegisterITCConfigChannel2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCConfigChannel2RuntimeParams structure as well.
  cmdTemplate = "ITCConfigChannel2/Z[=number:displayErrors]/S=number:scale/"
                "M=number:mode/A[=number:adjustRate]/O=number:overflow/"
                "U=number:underrun/D=number:decimate "
                "/DEV=number:deviceID/CHN=number:channelTypeNumeric/"
                "CHS=string:channelTypeString number:channelNumber, "
                "number:samplingInterval, wave:data";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCConfigChannel2RuntimeParams),
                           (void *) ExecuteITCConfigChannel2,
                           kOperationIsThreadSafe);
}

int RegisterITCConfigChannelReset2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCConfigChannelReset2RuntimeParams structure as well.
  cmdTemplate =
      "ITCConfigChannelReset2/Z[=number:displayErrors]/DEV=number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCConfigChannelReset2RuntimeParams),
                           (void *) ExecuteITCConfigChannelReset2,
                           kOperationIsThreadSafe);
}

int RegisterITCConfigChannelUpload2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCConfigChannelUpload2RuntimeParams structure as well.
  cmdTemplate =
      "ITCConfigChannelUpload2/Z[=number:displayErrors]/DEV=number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCConfigChannelUpload2RuntimeParams),
                           (void *) ExecuteITCConfigChannelUpload2,
                           kOperationIsThreadSafe);
}

int RegisterITCFIFOAvailable2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCFIFOAvailable2RuntimeParams structure as well.
  cmdTemplate = "ITCFIFOAvailable2/Z[=number:displayErrors] "
                "/DEV=number:deviceID/CHN=number:channelTypeNumeric/"
                "CHS=string:channelTypeString number:channelNumber";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCFIFOAvailable2RuntimeParams),
                           (void *) ExecuteITCFIFOAvailable2,
                           kOperationIsThreadSafe);
}

int RegisterITCFIFOAvailableAll2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCFIFOAvailableAll2RuntimeParams structure as well.
  cmdTemplate = "ITCFIFOAvailableAll2/Z[=number:displayErrors]/O/FREE/"
                "DEV=number:deviceID wave:{channelSelection,real}, "
                "DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCFIFOAvailableAll2RuntimeParams),
                           (void *) ExecuteITCFIFOAvailableAll2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetAllChannelsConfig2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetAllChannelsConfig2RuntimeParams structure as well.
  cmdTemplate = "ITCGetAllChannelsConfig2/Z[=number:displayErrors]/O/FREE/"
                "DEV=number:deviceID wave:{channelSelection,real}, "
                "DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetAllChannelsConfig2RuntimeParams),
                           (void *) ExecuteITCGetAllChannelsConfig2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetChannelConfig2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetChannelConfig2RuntimeParams structure as well.
  cmdTemplate = "ITCGetChannelConfig2/Z[=number:displayErrors] "
                "/DEV=number:deviceID/CHN=number:channelTypeNumeric/"
                "CHS=string:channelTypeString/O/FREE number:channelNumber, "
                "DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetChannelConfig2RuntimeParams),
                           (void *) ExecuteITCGetChannelConfig2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetCurrentDevice2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetCurrentDevice2RuntimeParams structure as well.
  cmdTemplate       = "ITCGetCurrentDevice2/Z[=number:displayErrors]";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetCurrentDevice2RuntimeParams),
                           (void *) ExecuteITCGetCurrentDevice2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetDeviceInfo2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetDeviceInfo2RuntimeParams structure as well.
  cmdTemplate = "ITCGetDeviceInfo2/Z[=number:displayErrors]/"
                "DEV=number:deviceID /O /FREE DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetDeviceInfo2RuntimeParams),
                           (void *) ExecuteITCGetDeviceInfo2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetDevices2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetDevices2RuntimeParams structure as well.
  cmdTemplate = "ITCGetDevices2/Z[=number:displayErrors]/"
                "DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetDevices2RuntimeParams),
                           (void *) ExecuteITCGetDevices2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetErrorString2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetErrorString2RuntimeParams structure as well.
  cmdTemplate = "ITCGetErrorString2/Z[=number:displayErrors]/"
                "DEV[=number:deviceID] /X number:errorCode";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = ERROR_RETURN_VARIABLE;
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetErrorString2RuntimeParams),
                           (void *) ExecuteITCGetErrorString2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetSerialNumber2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetSerialNumber2RuntimeParams structure as well.
  cmdTemplate = "ITCGetSerialNumber2/Z[=number:displayErrors]/"
                "DEV=number:deviceID/O/FREE DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetSerialNumber2RuntimeParams),
                           (void *) ExecuteITCGetSerialNumber2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetState2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetState2RuntimeParams structure as well.
  cmdTemplate = "ITCGetState2/Z[=number:displayErrors]/DEV=number:deviceID/R/"
                "OF/C/E/ALL /O/FREE DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetState2RuntimeParams),
                           (void *) ExecuteITCGetState2,
                           kOperationIsThreadSafe);
}

int RegisterITCGetVersions2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCGetVersions2RuntimeParams structure as well.
  cmdTemplate = "ITCGetVersions2/Z[=number:displayErrors]/DEV=number:deviceID "
                "/O /FREE DataFolderAndName:{dest,text}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCGetVersions2RuntimeParams),
                           (void *) ExecuteITCGetVersions2,
                           kOperationIsThreadSafe);
}

int RegisterITCInitialize2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCInitialize2RuntimeParams structure as well.
  cmdTemplate = "ITCInitialize2/Z[=number:displayErrors] "
                "/DEV=number:deviceID/M=number:mode/U=string:file/"
                "F=number:fFunc/D=number:dFunc/H=number:hFunc/R=number:rFunc";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCInitialize2RuntimeParams),
                           (void *) ExecuteITCInitialize2,
                           kOperationIsThreadSafe);
}

int RegisterITCOpenDevice2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCOpenDevice2RuntimeParams structure as well.
  cmdTemplate = "ITCOpenDevice2/Z[=number:displayErrors]/"
                "DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString "
                "number:deviceNumber";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCOpenDevice2RuntimeParams),
                           (void *) ExecuteITCOpenDevice2,
                           kOperationIsThreadSafe);
}

int RegisterITCReadADC2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCReadADC2RuntimeParams structure as well.
  cmdTemplate = "ITCReadADC2/Z[=number:displayErrors]/DEV=number:deviceID/"
                "V[=number:voltageOn]/C[=number:calibrateOn] "
                "number:channelNumber";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCReadADC2RuntimeParams),
                           (void *) ExecuteITCReadADC2, kOperationIsThreadSafe);
}

int RegisterITCReadDigital2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCReadDigital2RuntimeParams structure as well.
  cmdTemplate = "ITCReadDigital2/Z[=number:displayErrors]/DEV=number:deviceID "
                "number:channelNumber";
  runtimeNumVarList = EXCEPTION_VARS ";" RETURN_VARIABLE;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCReadDigital2RuntimeParams),
                           (void *) ExecuteITCReadDigital2,
                           kOperationIsThreadSafe);
}

int RegisterITCReadTimer2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCReadTimer2RuntimeParams structure as well.
  cmdTemplate = "ITCReadTimer2/Z[=number:displayErrors]/DEV=number:deviceID/O/"
                "FREE number:mode, DataFolderAndName:{dest,real}";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCReadTimer2RuntimeParams),
                           (void *) ExecuteITCReadTimer2,
                           kOperationIsThreadSafe);
}

int RegisterITCSelectDevice2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCSelectDevice2RuntimeParams structure as well.
  cmdTemplate = "ITCSelectDevice2/Z[=number:displayErrors] number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCSelectDevice2RuntimeParams),
                           (void *) ExecuteITCSelectDevice2,
                           kOperationIsThreadSafe);
}

int RegisterITCSetDAC2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCSetDAC2RuntimeParams structure as well.
  cmdTemplate = "ITCSetDAC2/Z[=number:displayErrors]/DEV=number:deviceID/"
                "V[=number:voltageOn]/C[=number:calibrateOn] "
                "number:channelNumber, number:voltageOrDACUnits";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCSetDAC2RuntimeParams),
                           (void *) ExecuteITCSetDAC2, kOperationIsThreadSafe);
}

int RegisterITCSetGlobals2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCSetGlobals2RuntimeParams structure as well.
  cmdTemplate =
      "ITCSetGlobals2/Z[=number:displayErrors]/D[=number:debugOutput]";
  runtimeNumVarList = "";
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCSetGlobals2RuntimeParams),
                           (void *) ExecuteITCSetGlobals2,
                           kOperationIsThreadSafe);
}

int RegisterITCSetModes2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCSetModes2RuntimeParams structure as well.
  cmdTemplate = "ITCSetModes2/Z[=number:displayErrors]/DEV=number:deviceID/"
                "I=number:DigitalInputMode/E=number:ExternalTriggerMode/"
                "T=number:ExternalTrigger/C[=number:EnableExternalClock]/"
                "D=number:DACShiftValue/R=number:InputRange/"
                "P=number:TriggerOutPosition/O[=number:OutputEnable]/"
                "F[=number:ResetFIFOFlag]/L=number:ControlLight/"
                "S=number:SamplingInterval";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCSetModes2RuntimeParams),
                           (void *) ExecuteITCSetModes2,
                           kOperationIsThreadSafe);
}

int RegisterITCSetState2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCSetState2RuntimeParams structure as well.
  cmdTemplate = "ITCSetState2/Z[=number:displayErrors]/S=number:switchCommand/"
                "DEV=number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCSetState2RuntimeParams),
                           (void *) ExecuteITCSetState2,
                           kOperationIsThreadSafe);
}

int RegisterITCStartAcq2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCStartAcq2RuntimeParams structure as well.
  cmdTemplate = "ITCStartAcq2/Z[=number:displayErrors]/DEV=number:deviceID/"
                "R[=number:resetFIFOs]/F[=number:fastPointerUpdate]/"
                "OE=number:outputEnable/EXT=number:externalTrigger/"
                "STPO=number:stopOnOverflow/STPU=number:stopOnUnderrun/"
                "STRT=number:startTime/STOP=number:stopTime";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCStartAcq2RuntimeParams),
                           (void *) ExecuteITCStartAcq2,
                           kOperationIsThreadSafe);
}

int RegisterITCStopAcq2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCStopAcq2RuntimeParams structure as well.
  cmdTemplate = "ITCStopAcq2/Z[=number:displayErrors]/DEV=number:deviceID";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCStopAcq2RuntimeParams),
                           (void *) ExecuteITCStopAcq2, kOperationIsThreadSafe);
}

int RegisterITCUpdateFIFOPositionAll2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCUpdateFIFOPositionAll2RuntimeParams structure as well.
  cmdTemplate = "ITCUpdateFIFOPositionAll2/Z[=number:displayErrors]/"
                "DEV=number:deviceID wave:fifoPos";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCUpdateFIFOPositionAll2RuntimeParams),
                           (void *) ExecuteITCUpdateFIFOPositionAll2,
                           kOperationIsThreadSafe);
}

int RegisterITCUpdateFIFOPosition2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCUpdateFIFOPosition2RuntimeParams structure as well.
  cmdTemplate = "ITCUpdateFIFOPosition2/Z[=number:displayErrors]/"
                "DEV=number:deviceID/L[=number:lastFIFO] /R[=number:resetFIFO] "
                "/CHN=number:channelTypeNumeric/CHS=string:channelTypeString "
                "number:channelNumber, number:position";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCUpdateFIFOPosition2RuntimeParams),
                           (void *) ExecuteITCUpdateFIFOPosition2,
                           kOperationIsThreadSafe);
}

int RegisterITCWriteDigital2(void)
{
  const char *cmdTemplate;
  const char *runtimeNumVarList;
  const char *runtimeStrVarList;

  // NOTE: If you change this template, you must change the
  // ITCWriteDigital2RuntimeParams structure as well.
  cmdTemplate = "ITCWriteDigital2/Z[=number:displayErrors] "
                "/DEV=number:deviceID number:channelNumber, number:value";
  runtimeNumVarList = EXCEPTION_VARS;
  runtimeStrVarList = "";
  return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList,
                           sizeof(ITCWriteDigital2RuntimeParams),
                           (void *) ExecuteITCWriteDigital2,
                           kOperationIsThreadSafe);
}

} // anonymous namespace

int RegisterOperations(void) // Register any operations with Igor.
{
  if(int result = RegisterITCCloseAll2())
  {
    return result;
  }

  if(int result = RegisterITCCloseDevice2())
  {
    return result;
  }

  if(int result = RegisterITCConfigChannelReset2())
  {
    return result;
  }

  if(int result = RegisterITCConfigChannelUpload2())
  {
    return result;
  }

  if(int result = RegisterITCConfigAllChannels2())
  {
    return result;
  }

  if(int result = RegisterITCConfigChannel2())
  {
    return result;
  }

  if(int result = RegisterITCFIFOAvailable2())
  {
    return result;
  }

  if(int result = RegisterITCFIFOAvailableAll2())
  {
    return result;
  }

  if(int result = RegisterITCGetAllChannelsConfig2())
  {
    return result;
  }

  if(int result = RegisterITCGetChannelConfig2())
  {
    return result;
  }

  if(int result = RegisterITCGetCurrentDevice2())
  {
    return result;
  }

  if(int result = RegisterITCGetDeviceInfo2())
  {
    return result;
  }

  if(int result = RegisterITCGetDevices2())
  {
    return result;
  }

  if(int result = RegisterITCGetErrorString2())
  {
    return result;
  }

  //	if (int result = RegisterITCGetLastError2()) {
  //		return result;
  //	}

  if(int result = RegisterITCGetSerialNumber2())
  {
    return result;
  }

  if(int result = RegisterITCGetState2())
  {
    return result;
  }

  if(int result = RegisterITCGetVersions2())
  {
    return result;
  }

  if(int result = RegisterITCInitialize2())
  {
    return result;
  }

  if(int result = RegisterITCOpenDevice2())
  {
    return result;
  }

  if(int result = RegisterITCReadADC2())
  {
    return result;
  }

  if(int result = RegisterITCReadDigital2())
  {
    return result;
  }

  if(int result = RegisterITCReadTimer2())
  {
    return result;
  }

  if(int result = RegisterITCSelectDevice2())
  {
    return result;
  }

  if(int result = RegisterITCSetDAC2())
  {
    return result;
  }

  if(int result = RegisterITCSetGlobals2())
  {
    return result;
  }

  if(int result = RegisterITCSetModes2())
  {
    return result;
  }

  if(int result = RegisterITCSetState2())
  {
    return result;
  }

  //	if (int result = RegisterITCShortAcquisition2()) {
  //		return result;
  //	}

  if(int result = RegisterITCStartAcq2())
  {
    return result;
  }

  if(int result = RegisterITCStopAcq2())
  {
    return result;
  }

  if(int result = RegisterITCUpdateFIFOPosition2())
  {
    return result;
  }

  if(int result = RegisterITCUpdateFIFOPositionAll2())
  {
    return result;
  }

  if(int result = RegisterITCWriteDigital2())
  {
    return result;
  }

  // There are no more operations added by this XOP.
  return 0;
}
