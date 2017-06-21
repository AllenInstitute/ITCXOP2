#pragma once

#ifndef ITCDLL_H
#define ITCDLL_H

#include <vector>
#include <Windows.h>

#include "ITCDeviceTypeEnum.h"
#include "ITCChannelTypeEnum.h"
#include "ErrorDisplayClass.h"
#include "DeviceIDHelper.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// @brief Wrapper class for ITC DLL functions
class ITCDLL
{
public:
  static void ITC_AnalyzeError(LONG Status, char *Text, DWORD MaxCharacters);
  static void ITC_AsyncIO(const DeviceIDHelper &DeviceID,
                          std::vector<ITCChannelDataEx> *Channels);
  static void ITC_CloseDevice(const DeviceIDHelper &DeviceID);
  static void ITC_CloseDevice(HANDLE DeviceHandle);
  static void ITC_ConfigDevice(const DeviceIDHelper &DeviceID,
                               ITCPublicConfig *sITCConfig);
  static void ITC_ConfigDevice(HANDLE DeviceHandle,
                               ITCPublicConfig *sITCConfig);
  static void ITC_Devices(ITCDeviceTypeEnum DeviceType, DWORD *DeviceNumber);
  static void ITC_GetChannels(const DeviceIDHelper &DeviceID,
                              std::vector<ITCChannelInfo> *Channels);
  static void ITC_GetDataAvailable(const DeviceIDHelper &DeviceID,
                                   std::vector<ITCChannelDataEx> *Channels);
  static void ITC_GetDeviceInfo(const DeviceIDHelper &DeviceID,
                                GlobalDeviceInfo *sDeviceInfo);
  static void ITC_GetDeviceType(const DeviceIDHelper &DeviceID,
                                ITCDeviceTypeEnum *DeviceType,
                                DWORD *DeviceNumber);
  static void ITC_GetState(const DeviceIDHelper &DeviceID,
                           ITCStatus *lITCStatus);
  static void ITC_GetStatusText(HANDLE DeviceHandle, LONG Status, char *Text,
                                DWORD MaxCharacters);
  static void ITC_GetVersions(const DeviceIDHelper &DeviceID,
                              VersionInfo *ThisDriverVersion,
                              VersionInfo *KernelLevelDriverVersion,
                              VersionInfo *HardwareVersion);
  static void ITC_GlobalConfig(ITCGlobalConfig *GlobalConfig);
  static void ITC_InitDevice(const DeviceIDHelper &DeviceID,
                             HWFunction *sHWFunction);
  static void ITC_OpenDevice(ITCDeviceTypeEnum DeviceType, DWORD DeviceNumber,
                             DWORD Mode, HANDLE *DeviceHandle);
  static void ITC_ResetChannels(const DeviceIDHelper &DeviceID);
  static void ITC_SetChannels(const DeviceIDHelper &DeviceID,
                              std::vector<ITCChannelInfo> Channels);
  static void ITC_SetSoftKey(const DeviceIDHelper &DeviceID, DWORD SoftKey);
  static void ITC_SetState(const DeviceIDHelper &DeviceID,
                           ITCStatus *lITCStatus);
  static void ITC_Start(const DeviceIDHelper &DeviceID, ITCStartInfo *sParam);
  static void ITC_Stop(const DeviceIDHelper &DeviceID, void *sParam);
  static void ITC_UpdateChannels(const DeviceIDHelper &DeviceID);
  static void
  ITC_UpdateFIFOPosition(const DeviceIDHelper &DeviceID,
                         std::vector<ITCChannelDataEx> channelDataEx);
};

#endif // ITCDLL_H
