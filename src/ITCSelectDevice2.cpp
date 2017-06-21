#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template: ITCSelectDevice2/Z[=number:displayErrors] number:deviceID
extern "C" int ExecuteITCSelectDevice2(ITCSelectDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Check the validity of the device ID by constructing a deviceID object
  size_t currentDeviceID = lockToIntegerRange<size_t>(p->deviceID);
  DeviceIDHelper DeviceID(currentDeviceID);

  // Set the current thread device ID
  DeviceIDHelper::setCurrentThreadDeviceID(currentDeviceID);

  END_OUTER_CATCH
}
