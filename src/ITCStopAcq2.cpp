#include "ITC_StandardHeaders.h"

// Operation template: ITCStopAcq2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int ExecuteITCStopAcq2(ITCStopAcq2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  ITCDLL::ITC_Stop(DeviceID, nullptr);

  END_OUTER_CATCH
}
