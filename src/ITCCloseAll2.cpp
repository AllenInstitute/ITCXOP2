#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"
#include "HelperFunctions_ITC.h"

#include <array>

#include "fmt/format.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

extern DeviceIDClass DeviceIDs;

// Operation template: ITCCloseAll2/Z[=number:displayErrors]

void CloseAllDevices()
{
  const std::array<int, 6> deviceTypes{ITC16_ID, ITC18_ID, USB16_ID,
                                       USB18_ID, ITC00_ID, ITC1600_ID};
  for(auto dt : deviceTypes)
  {
    for(int i = 0; i < ITC_MAX_DEVICE_NUMBER; i += 1)
    {
      HANDLE handle = INVALID_HANDLE_VALUE;
      DWORD ret     = ITC_GetDeviceHandle(dt, i, &handle);

      if(ret != ERROR_SUCCESS || handle == INVALID_HANDLE_VALUE)
      {
        continue;
      }

      try
      {
        CloseDeviceLowLevel(handle);
      }
      catch(const ITCException &e)
      {
        DebugOut("CloseAllDevices",
                 fmt::format("Unexpected exception {} for device "
                             "type {} and number {}",
                             e, dt, i));
      }
    }
  }

  // clear our list of open devices
  // after this all devices are removed and cleared
  for(size_t currDeviceID = 0; currDeviceID < DeviceIDClass::MaxNumberOfDevices;
      currDeviceID++)
  {
    DeviceIDs.forceRemove(currDeviceID);
    DeviceIDs.forceRelease(currDeviceID);
  }

  DeviceIDHelper::setCurrentThreadDeviceID(DeviceIDClass::INVALID_DEVICE_INDEX);
}

extern "C" int ExecuteITCCloseAll2(ITCCloseAll2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  CloseAllDevices();

  END_OUTER_CATCH
}
