#include "ITC_StandardHeaders.h"
#include "HelperFunctions_ITC.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Global variables
// Defined in itcXOP2.cpp
extern DeviceIDClass DeviceIDs;

// Operation template:
// ITCOpenDevice2/Z[=number:displayErrors]/DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString
// number:deviceNumber

extern "C" int ExecuteITCOpenDevice2(ITCOpenDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Before we open the device, set v_Value to the invalid device ID.
  // The value will be updated later.
  SetOperationNumVar(RETURN_VARIABLE, DeviceIDClass::INVALID_DEVICE_ID);

  // Get the device type from the DTN or DTS flag
  ITCDeviceTypeEnum DeviceType = GetDeviceTypeFromParameters(p);

  // Main parameters.
  DWORD DeviceNumber = 0;
  if(p->deviceNumberEncountered)
  {
    // Parameter: p->deviceNumber
    DeviceNumber = lockToIntegerRange<DWORD>(p->deviceNumber);
  }

  // Open the device
  HANDLE currDeviceHandle = nullptr;

  ITCDLL::ITC_OpenDevice(DeviceType, DeviceNumber, SMART_MODE,
                         &currDeviceHandle);

  size_t currentDeviceID = DeviceIDs.store(currDeviceHandle);

  // Now open an actual DeviceIDHelper
  DeviceIDHelper DeviceID(currentDeviceID);
  DeviceIDHelper::setCurrentThreadDeviceID(currentDeviceID);

  try
  {
    // Do ITC_GlobalConfig
    //
    // NOTE: Igor will provide the XOP with pointers to individual FIFO.
    // The internal FIFOs are not used.
    // Disable the internal FIFOs using ITC_GlobalConfig to save memory
    ITCGlobalConfig currGlobalConfig;
    ZeroMemory(&currGlobalConfig, sizeof(currGlobalConfig));
    currGlobalConfig.SoftwareFIFOSize = 0;
    ITCDLL::ITC_GlobalConfig(&currGlobalConfig);

    // Initialize the device
    ITCDLL::ITC_InitDevice(DeviceID, NULL);

    const long key = (InstruKey << 16) | IgorKey;
    ITCDLL::ITC_SetSoftKey(DeviceID, key);
  }
  catch(const std::exception &e)
  {
    CloseDevice(DeviceID);
    throw e;
  }

  // Set the Device ID in V_value
  if (int RetVal = SetOperationNumVar(RETURN_VARIABLE, (double)currentDeviceID))
  {
    throw IgorException(RetVal);
  }

  END_OUTER_CATCH
}
