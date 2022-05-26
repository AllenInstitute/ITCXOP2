#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCReadDigital2/Z[=number:displayErrors]/DEV=number:deviceID
// number:channelNumber

extern "C" int ExecuteITCReadDigital2(ITCReadDigital2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  SetOperationReturn(RETURN_VARIABLE, DOUBLE_NAN);

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);
  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  lITCChannelData[0].ChannelType = DIGITAL_INPUT;

  ITCDLL::ITC_AsyncIO(DeviceID, &lITCChannelData);

  SetOperationReturn(RETURN_VARIABLE, (double) lITCChannelData[0].Value);

  END_OUTER_CATCH
}
