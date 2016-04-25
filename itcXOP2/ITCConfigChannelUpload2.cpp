#include "ITC_StandardHeaders.h"

// Operation template:
// ITCConfigChannelUpload2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int
ExecuteITCConfigChannelUpload2(ITCConfigChannelUpload2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  DeviceIDHelper DeviceID(p);

  ITCDLL::ITC_UpdateChannels(DeviceID);

  END_OUTER_CATCH
}
