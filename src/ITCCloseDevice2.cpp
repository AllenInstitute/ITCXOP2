#include "ITC_StandardHeaders.h"
#include "HelperFunctions_ITC.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

extern DeviceIDClass DeviceIDs;

// Operation template:
// ITCCloseDevice2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int ExecuteITCCloseDevice2(ITCCloseDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  CloseDevice(DeviceID);

  END_OUTER_CATCH
}
