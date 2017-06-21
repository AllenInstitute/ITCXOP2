#pragma once

#include "ITC_StandardHeaders.h"
#include <Windows.h>
#include "itcmm.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

enum class ITCDeviceTypeEnum : DWORD
{
  ITC16    = ITC16_ID,
  ITC18    = ITC18_ID,
  ITC1600  = ITC1600_ID,
  ITC00    = ITC00_ID,
  ITC16USB = USB16_ID,
  ITC18USB = USB18_ID,
  ITCALL   = (DWORD) ITC_ALL_DEVICES_ID
};

ITCDeviceTypeEnum MakeITCDeviceEnum(DWORD DeviceTypeInt);

/// @brief Converts a device type number to an ITCDeviceEnum value
///
/// Performs bounds checking on the allowed values.
///
/// @param[in] deviceTypeNumeric -- device type number passed from Igor
///
/// @return DeviceType -- converted value
///
/// @throw Igor exception on invalid device type
ITCDeviceTypeEnum GetNumericDeviceType(double deviceTypeNumeric);

/// @brief Converts a device type string to an ITCDeviceEnum value
///
/// Throws an exception if the device type string does not match
/// one of the acceptable values
///
/// @param[in] deviceTypeString -- device type string passed from Igor
///
/// @return DeviceType -- converted value
///
/// @throw Igor exception on invalid device type
ITCDeviceTypeEnum GetStringDeviceType(Handle deviceTypeString);

/// @brief Read device type from /DTN and /DTS operation flags
///
/// @param p -- Operation parameter struct
///
/// @return DeviceType -- converted value
///
/// @throw Igor exception on invalid flags
template <typename T>
ITCDeviceTypeEnum GetDeviceTypeFromParameters(T *p)
{
  if(p->DTNFlagEncountered && p->DTSFlagEncountered)
  {
    ITCDeviceTypeEnum deviceTypeNumeric =
        GetNumericDeviceType(p->deviceTypeNumeric);
    ITCDeviceTypeEnum deviceTypeString =
        GetStringDeviceType(p->deviceTypeString);
    if(deviceTypeNumeric == deviceTypeString)
    {
      return deviceTypeNumeric;
    }
    else
    {
      // DTN and DTS don't match
      throw IgorException(DTN_DTS_DISAGREE);
    }
  }

  if(p->DTNFlagEncountered)
  {
    return GetNumericDeviceType(p->deviceTypeNumeric);
  }

  if(p->DTSFlagEncountered)
  {
    return GetStringDeviceType(p->deviceTypeString);
  }

  // Neither DTN nor DTS encountered.
  // Default to ITCALL.
  return ITCDeviceTypeEnum::ITCALL;
}
