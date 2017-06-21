#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template: ITCStopAcq2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int ExecuteITCStopAcq2(ITCStopAcq2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  ITCDLL::ITC_Stop(DeviceID, nullptr);

  END_OUTER_CATCH
}
