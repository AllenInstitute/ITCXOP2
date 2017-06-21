#pragma once

#include "XOPStandardHeaders.h"
#include "CustomExceptions.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#pragma pack(2) // All structures passed to Igor are two-byte aligned.

struct ITCCloseAll2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCCloseAll2RuntimeParams ITCCloseAll2RuntimeParams;
typedef struct ITCCloseAll2RuntimeParams *ITCCloseAll2RuntimeParamsPtr;

struct ITCCloseDevice2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCCloseDevice2RuntimeParams ITCCloseDevice2RuntimeParams;
typedef struct ITCCloseDevice2RuntimeParams *ITCCloseDevice2RuntimeParamsPtr;

struct ITCConfigAllChannels2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int configEncountered;
  waveHndl config;
  int configParamsSet[1];

  // Parameters for simple main group #1.
  int dataEncountered;
  waveHndl data;
  int dataParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCConfigAllChannels2RuntimeParams
    ITCConfigAllChannels2RuntimeParams;
typedef struct ITCConfigAllChannels2RuntimeParams
    *ITCConfigAllChannels2RuntimeParamsPtr;

struct ITCConfigChannel2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /S flag group.
  int SFlagEncountered;
  double scale;
  int SFlagParamsSet[1];

  // Parameters for /M flag group.
  int MFlagEncountered;
  double mode;
  int MFlagParamsSet[1];

  // Parameters for /A flag group.
  int AFlagEncountered;
  double adjustRate; // Optional parameter.
  int AFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  double overflow;
  int OFlagParamsSet[1];

  // Parameters for /U flag group.
  int UFlagEncountered;
  double underrun;
  int UFlagParamsSet[1];

  // Parameters for /D flag group.
  int DFlagEncountered;
  double decimate;
  int DFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /CHN flag group.
  int CHNFlagEncountered;
  double channelTypeNumeric;
  int CHNFlagParamsSet[1];

  // Parameters for /CHS flag group.
  int CHSFlagEncountered;
  Handle channelTypeString;
  int CHSFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // Parameters for simple main group #1.
  int samplingIntervalEncountered;
  double samplingInterval;
  int samplingIntervalParamsSet[1];

  // Parameters for simple main group #2.
  int dataEncountered;
  waveHndl data;
  int dataParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCConfigChannel2RuntimeParams ITCConfigChannel2RuntimeParams;
typedef struct ITCConfigChannel2RuntimeParams
    *ITCConfigChannel2RuntimeParamsPtr;

struct ITCConfigChannelReset2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCConfigChannelReset2RuntimeParams
    ITCConfigChannelReset2RuntimeParams;
typedef struct ITCConfigChannelReset2RuntimeParams
    *ITCConfigChannelReset2RuntimeParamsPtr;

struct ITCConfigChannelUpload2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCConfigChannelUpload2RuntimeParams
    ITCConfigChannelUpload2RuntimeParams;
typedef struct ITCConfigChannelUpload2RuntimeParams
    *ITCConfigChannelUpload2RuntimeParamsPtr;

struct ITCFIFOAvailable2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /CHN flag group.
  int CHNFlagEncountered;
  double channelTypeNumeric;
  int CHNFlagParamsSet[1];

  // Parameters for /CHS flag group.
  int CHSFlagEncountered;
  Handle channelTypeString;
  int CHSFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCFIFOAvailable2RuntimeParams ITCFIFOAvailable2RuntimeParams;
typedef struct ITCFIFOAvailable2RuntimeParams
    *ITCFIFOAvailable2RuntimeParamsPtr;

struct ITCFIFOAvailableAll2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelSelectionEncountered;
  waveHndl channelSelection;
  int channelSelectionParamsSet[1];

  // Parameters for simple main group #1.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCFIFOAvailableAll2RuntimeParams
    ITCFIFOAvailableAll2RuntimeParams;
typedef struct ITCFIFOAvailableAll2RuntimeParams
    *ITCFIFOAvailableAll2RuntimeParamsPtr;

struct ITCGetAllChannelsConfig2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelSelectionEncountered;
  waveHndl channelSelection;
  int channelSelectionParamsSet[1];

  // Parameters for simple main group #1.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetAllChannelsConfig2RuntimeParams
    ITCGetAllChannelsConfig2RuntimeParams;
typedef struct ITCGetAllChannelsConfig2RuntimeParams
    *ITCGetAllChannelsConfig2RuntimeParamsPtr;

struct ITCGetChannelConfig2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /CHN flag group.
  int CHNFlagEncountered;
  double channelTypeNumeric;
  int CHNFlagParamsSet[1];

  // Parameters for /CHS flag group.
  int CHSFlagEncountered;
  Handle channelTypeString;
  int CHSFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // Parameters for simple main group #1.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetChannelConfig2RuntimeParams
    ITCGetChannelConfig2RuntimeParams;
typedef struct ITCGetChannelConfig2RuntimeParams
    *ITCGetChannelConfig2RuntimeParamsPtr;

struct ITCGetCurrentDevice2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetCurrentDevice2RuntimeParams
    ITCGetCurrentDevice2RuntimeParams;
typedef struct ITCGetCurrentDevice2RuntimeParams
    *ITCGetCurrentDevice2RuntimeParamsPtr;

struct ITCGetDeviceInfo2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetDeviceInfo2RuntimeParams ITCGetDeviceInfo2RuntimeParams;
typedef struct ITCGetDeviceInfo2RuntimeParams
    *ITCGetDeviceInfo2RuntimeParamsPtr;

struct ITCGetDevices2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DTN flag group.
  int DTNFlagEncountered;
  double deviceTypeNumeric;
  int DTNFlagParamsSet[1];

  // Parameters for /DTS flag group.
  int DTSFlagEncountered;
  Handle deviceTypeString;
  int DTSFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetDevices2RuntimeParams ITCGetDevices2RuntimeParams;
typedef struct ITCGetDevices2RuntimeParams *ITCGetDevices2RuntimeParamsPtr;

struct ITCGetErrorString2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID; // Optional parameter.
  int DEVFlagParamsSet[1];

  // Parameters for /X flag group.
  int XFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int errorCodeEncountered;
  double errorCode;
  int errorCodeParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetErrorString2RuntimeParams ITCGetErrorString2RuntimeParams;
typedef struct ITCGetErrorString2RuntimeParams
    *ITCGetErrorString2RuntimeParamsPtr;

struct ITCGetSerialNumber2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetSerialNumber2RuntimeParams
    ITCGetSerialNumber2RuntimeParams;
typedef struct ITCGetSerialNumber2RuntimeParams
    *ITCGetSerialNumber2RuntimeParamsPtr;

struct ITCGetState2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /R flag group.
  int RFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /OF flag group.
  int OFFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /C flag group.
  int CFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /E flag group.
  int EFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /ALL flag group.
  int ALLFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetState2RuntimeParams ITCGetState2RuntimeParams;
typedef struct ITCGetState2RuntimeParams *ITCGetState2RuntimeParamsPtr;

struct ITCGetVersions2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCGetVersions2RuntimeParams ITCGetVersions2RuntimeParams;
typedef struct ITCGetVersions2RuntimeParams *ITCGetVersions2RuntimeParamsPtr;

struct ITCInitialize2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /M flag group.
  int MFlagEncountered;
  double mode;
  int MFlagParamsSet[1];

  // Parameters for /U flag group.
  int UFlagEncountered;
  Handle file;
  int UFlagParamsSet[1];

  // Parameters for /F flag group.
  int FFlagEncountered;
  double fFunc;
  int FFlagParamsSet[1];

  // Parameters for /D flag group.
  int DFlagEncountered;
  double dFunc;
  int DFlagParamsSet[1];

  // Parameters for /H flag group.
  int HFlagEncountered;
  double hFunc;
  int HFlagParamsSet[1];

  // Parameters for /R flag group.
  int RFlagEncountered;
  double rFunc;
  int RFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCInitialize2RuntimeParams ITCInitialize2RuntimeParams;
typedef struct ITCInitialize2RuntimeParams *ITCInitialize2RuntimeParamsPtr;

struct ITCOpenDevice2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DTN flag group.
  int DTNFlagEncountered;
  double deviceTypeNumeric;
  int DTNFlagParamsSet[1];

  // Parameters for /DTS flag group.
  int DTSFlagEncountered;
  Handle deviceTypeString;
  int DTSFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int deviceNumberEncountered;
  double deviceNumber;
  int deviceNumberParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCOpenDevice2RuntimeParams ITCOpenDevice2RuntimeParams;
typedef struct ITCOpenDevice2RuntimeParams *ITCOpenDevice2RuntimeParamsPtr;

struct ITCReadADC2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /V flag group.
  int VFlagEncountered;
  double voltageOn; // Optional parameter.
  int VFlagParamsSet[1];

  // Parameters for /C flag group.
  int CFlagEncountered;
  double calibrateOn; // Optional parameter.
  int CFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCReadADC2RuntimeParams ITCReadADC2RuntimeParams;
typedef struct ITCReadADC2RuntimeParams *ITCReadADC2RuntimeParamsPtr;

struct ITCReadDigital2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCReadDigital2RuntimeParams ITCReadDigital2RuntimeParams;
typedef struct ITCReadDigital2RuntimeParams *ITCReadDigital2RuntimeParamsPtr;

struct ITCReadTimer2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Parameters for /FREE flag group.
  int FREEFlagEncountered;
  // There are no fields for this group because it has no parameters.

  // Main parameters.

  // Parameters for simple main group #0.
  int modeEncountered;
  double mode;
  int modeParamsSet[1];

  // Parameters for simple main group #1.
  int destEncountered;
  DataFolderAndName dest;
  int destParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCReadTimer2RuntimeParams ITCReadTimer2RuntimeParams;
typedef struct ITCReadTimer2RuntimeParams *ITCReadTimer2RuntimeParamsPtr;

struct ITCSelectDevice2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int deviceIDEncountered;
  double deviceID;
  int deviceIDParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCSelectDevice2RuntimeParams ITCSelectDevice2RuntimeParams;
typedef struct ITCSelectDevice2RuntimeParams *ITCSelectDevice2RuntimeParamsPtr;

struct ITCSetDAC2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /V flag group.
  int VFlagEncountered;
  double voltageOn; // Optional parameter.
  int VFlagParamsSet[1];

  // Parameters for /C flag group.
  int CFlagEncountered;
  double calibrateOn; // Optional parameter.
  int CFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // Parameters for simple main group #1.
  int voltageOrDACUnitsEncountered;
  double voltageOrDACUnits;
  int voltageOrDACUnitsParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCSetDAC2RuntimeParams ITCSetDAC2RuntimeParams;
typedef struct ITCSetDAC2RuntimeParams *ITCSetDAC2RuntimeParamsPtr;

struct ITCSetGlobals2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /D flag group.
  int DFlagEncountered;
  double debugOutput; // Optional parameter.
  int DFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCSetGlobals2RuntimeParams ITCSetGlobals2RuntimeParams;
typedef struct ITCSetGlobals2RuntimeParams *ITCSetGlobals2RuntimeParamsPtr;

struct ITCSetModes2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /I flag group.
  int IFlagEncountered;
  double DigitalInputMode;
  int IFlagParamsSet[1];

  // Parameters for /E flag group.
  int EFlagEncountered;
  double ExternalTriggerMode;
  int EFlagParamsSet[1];

  // Parameters for /T flag group.
  int TFlagEncountered;
  double ExternalTrigger;
  int TFlagParamsSet[1];

  // Parameters for /C flag group.
  int CFlagEncountered;
  double EnableExternalClock; // Optional parameter.
  int CFlagParamsSet[1];

  // Parameters for /D flag group.
  int DFlagEncountered;
  double DACShiftValue;
  int DFlagParamsSet[1];

  // Parameters for /R flag group.
  int RFlagEncountered;
  double InputRange;
  int RFlagParamsSet[1];

  // Parameters for /P flag group.
  int PFlagEncountered;
  double TriggerOutPosition;
  int PFlagParamsSet[1];

  // Parameters for /O flag group.
  int OFlagEncountered;
  double OutputEnable; // Optional parameter.
  int OFlagParamsSet[1];

  // Parameters for /F flag group.
  int FFlagEncountered;
  double ResetFIFOFlag; // Optional parameter.
  int FFlagParamsSet[1];

  // Parameters for /L flag group.
  int LFlagEncountered;
  double ControlLight;
  int LFlagParamsSet[1];

  // Parameters for /S flag group.
  int SFlagEncountered;
  double SamplingInterval;
  int SFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCSetModes2RuntimeParams ITCSetModes2RuntimeParams;
typedef struct ITCSetModes2RuntimeParams *ITCSetModes2RuntimeParamsPtr;

struct ITCSetState2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /S flag group.
  int SFlagEncountered;
  double switchCommand;
  int SFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCSetState2RuntimeParams ITCSetState2RuntimeParams;
typedef struct ITCSetState2RuntimeParams *ITCSetState2RuntimeParamsPtr;

struct ITCStartAcq2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /R flag group.
  int RFlagEncountered;
  double resetFIFOs; // Optional parameter.
  int RFlagParamsSet[1];

  // Parameters for /F flag group.
  int FFlagEncountered;
  double fastPointerUpdate; // Optional parameter.
  int FFlagParamsSet[1];

  // Parameters for /OE flag group.
  int OEFlagEncountered;
  double outputEnable;
  int OEFlagParamsSet[1];

  // Parameters for /EXT flag group.
  int EXTFlagEncountered;
  double externalTrigger;
  int EXTFlagParamsSet[1];

  // Parameters for /STPO flag group.
  int STPOFlagEncountered;
  double stopOnOverflow;
  int STPOFlagParamsSet[1];

  // Parameters for /STPU flag group.
  int STPUFlagEncountered;
  double stopOnUnderrun;
  int STPUFlagParamsSet[1];

  // Parameters for /STRT flag group.
  int STRTFlagEncountered;
  double startTime;
  int STRTFlagParamsSet[1];

  // Parameters for /STOP flag group.
  int STOPFlagEncountered;
  double stopTime;
  int STOPFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCStartAcq2RuntimeParams ITCStartAcq2RuntimeParams;
typedef struct ITCStartAcq2RuntimeParams *ITCStartAcq2RuntimeParamsPtr;

struct ITCStopAcq2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCStopAcq2RuntimeParams ITCStopAcq2RuntimeParams;
typedef struct ITCStopAcq2RuntimeParams *ITCStopAcq2RuntimeParamsPtr;

struct ITCUpdateFIFOPosition2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Parameters for /L flag group.
  int LFlagEncountered;
  double lastFIFO; // Optional parameter.
  int LFlagParamsSet[1];

  // Parameters for /R flag group.
  int RFlagEncountered;
  double resetFIFO; // Optional parameter.
  int RFlagParamsSet[1];

  // Parameters for /CHN flag group.
  int CHNFlagEncountered;
  double channelTypeNumeric;
  int CHNFlagParamsSet[1];

  // Parameters for /CHS flag group.
  int CHSFlagEncountered;
  Handle channelTypeString;
  int CHSFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // Parameters for simple main group #1.
  int positionEncountered;
  double position;
  int positionParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCUpdateFIFOPosition2RuntimeParams
    ITCUpdateFIFOPosition2RuntimeParams;
typedef struct ITCUpdateFIFOPosition2RuntimeParams
    *ITCUpdateFIFOPosition2RuntimeParamsPtr;

struct ITCUpdateFIFOPositionAll2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int fifoPosEncountered;
  waveHndl fifoPos;
  int fifoPosParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCUpdateFIFOPositionAll2RuntimeParams
    ITCUpdateFIFOPositionAll2RuntimeParams;
typedef struct ITCUpdateFIFOPositionAll2RuntimeParams
    *ITCUpdateFIFOPositionAll2RuntimeParamsPtr;

struct ITCWriteDigital2RuntimeParams
{
  // Flag parameters.

  // Parameters for /Z flag group.
  int ZFlagEncountered;
  double displayErrors; // Optional parameter.
  int ZFlagParamsSet[1];

  // Parameters for /DEV flag group.
  int DEVFlagEncountered;
  double deviceID;
  int DEVFlagParamsSet[1];

  // Main parameters.

  // Parameters for simple main group #0.
  int channelNumberEncountered;
  double channelNumber;
  int channelNumberParamsSet[1];

  // Parameters for simple main group #1.
  int valueEncountered;
  double value;
  int valueParamsSet[1];

  // These are postamble fields that Igor sets.
  int calledFromFunction; // 1 if called from a user function, 0 otherwise.
  int calledFromMacro;    // 1 if called from a macro, 0 otherwise.
  UserFunctionThreadInfoPtr
      tp; // If not null, we are running from a ThreadSafe function.
};
typedef struct ITCWriteDigital2RuntimeParams ITCWriteDigital2RuntimeParams;
typedef struct ITCWriteDigital2RuntimeParams *ITCWriteDigital2RuntimeParamsPtr;

#pragma pack() // Reset structure alignment to default.

int RegisterOperations();

extern "C" {
int ExecuteITCCloseAll2(ITCCloseAll2RuntimeParamsPtr p);
int ExecuteITCCloseDevice2(ITCCloseDevice2RuntimeParamsPtr p);
int ExecuteITCConfigAllChannels2(ITCConfigAllChannels2RuntimeParamsPtr p);
int ExecuteITCConfigChannel2(ITCConfigChannel2RuntimeParamsPtr p);
int ExecuteITCConfigChannelReset2(ITCConfigChannelReset2RuntimeParamsPtr p);
int ExecuteITCConfigChannelUpload2(ITCConfigChannelUpload2RuntimeParamsPtr p);
int ExecuteITCFIFOAvailable2(ITCFIFOAvailable2RuntimeParamsPtr p);
int ExecuteITCFIFOAvailableAll2(ITCFIFOAvailableAll2RuntimeParamsPtr p);
int ExecuteITCGetAllChannelsConfig2(ITCGetAllChannelsConfig2RuntimeParamsPtr p);
int ExecuteITCGetChannelConfig2(ITCGetChannelConfig2RuntimeParamsPtr p);
int ExecuteITCGetCurrentDevice2(ITCGetCurrentDevice2RuntimeParamsPtr p);
int ExecuteITCGetDeviceInfo2(ITCGetDeviceInfo2RuntimeParamsPtr p);
int ExecuteITCGetDevices2(ITCGetDevices2RuntimeParamsPtr p);
int ExecuteITCGetErrorString2(ITCGetErrorString2RuntimeParamsPtr p);
int ExecuteITCGetSerialNumber2(ITCGetSerialNumber2RuntimeParamsPtr p);
int ExecuteITCGetState2(ITCGetState2RuntimeParamsPtr p);
int ExecuteITCGetVersions2(ITCGetVersions2RuntimeParamsPtr p);
int ExecuteITCInitialize2(ITCInitialize2RuntimeParamsPtr p);
int ExecuteITCOpenDevice2(ITCOpenDevice2RuntimeParamsPtr p);
int ExecuteITCReadADC2(ITCReadADC2RuntimeParamsPtr p);
int ExecuteITCReadDigital2(ITCReadDigital2RuntimeParamsPtr p);
int ExecuteITCReadTimer2(ITCReadTimer2RuntimeParamsPtr p);
int ExecuteITCSelectDevice2(ITCSelectDevice2RuntimeParamsPtr p);
int ExecuteITCSetDAC2(ITCSetDAC2RuntimeParamsPtr p);
int ExecuteITCSetGlobals2(ITCSetGlobals2RuntimeParamsPtr p);
int ExecuteITCSetModes2(ITCSetModes2RuntimeParamsPtr p);
int ExecuteITCSetState2(ITCSetState2RuntimeParamsPtr p);
int ExecuteITCStartAcq2(ITCStartAcq2RuntimeParamsPtr p);
int ExecuteITCStopAcq2(ITCStopAcq2RuntimeParamsPtr p);
int ExecuteITCUpdateFIFOPosition2(ITCUpdateFIFOPosition2RuntimeParamsPtr p);
int ExecuteITCUpdateFIFOPositionAll2(
    ITCUpdateFIFOPositionAll2RuntimeParamsPtr p);
int ExecuteITCWriteDigital2(ITCWriteDigital2RuntimeParamsPtr p);
}
