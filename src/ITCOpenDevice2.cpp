#include "ITC_StandardHeaders.h"
#include "HelperFunctions_ITC.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Global variables
// Defined in itcXOP2.cpp
extern DeviceIDClass DeviceIDs;

const int NUM_INIT_TRIALS = 10;

// Operation template:
// ITCOpenDevice2/Z[=number:displayErrors]/DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString
// number:deviceNumber

extern "C" int ExecuteITCOpenDevice2(ITCOpenDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Before we open the device, set v_Value to the invalid device ID.
  SetOperationReturn(RETURN_VARIABLE, DeviceIDClass::INVALID_DEVICE_ID);

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
    // Some ITC1600 have problems with initialization
    // and output
    //
    // > Error in call to ITC_InitDevice.
    // > Reported error: Could not load rack FPGA (0x8A513000)
    // > DLL Error.
    //
    // when opening/closing the device too fast in a loop.
    //
    // We now therefore redo the initialization in this case.
    for(int i = 0;; i++)
    {
      try
      {
        ITCDLL::ITC_InitDevice(DeviceID, NULL);
        break;
      }
      catch(const ITCException &e)
      {
        if(DeviceType == ITCDeviceTypeEnum::ITC1600 &&
           e.ErrorCode == 0x8A513000 && i < NUM_INIT_TRIALS)
        {
          DebugOut("ExecuteITCOpenDevice2",
                   "Redoing initialization: " + std::to_string(i));
          Sleep(100);
          continue;
        }

        throw e;
      }
    }

    const long key = (InstruKey << 16) | IgorKey;
    ITCDLL::ITC_SetSoftKey(DeviceID, key);
  }
  catch(const std::exception &e)
  {
    CloseDevice(DeviceID);
    throw e;
  }

  SetOperationReturn(RETURN_VARIABLE, (double) currentDeviceID);

  END_OUTER_CATCH
}
