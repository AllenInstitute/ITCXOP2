#include "ITC_StandardHeaders.h"

// Operation template:
// ITCConfigChannelReset2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int
ExecuteITCConfigChannelReset2(ITCConfigChannelReset2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  DeviceIDHelper DeviceID(p);

  ITCDLL::ITC_ResetChannels(DeviceID);

  END_OUTER_CATCH
}
