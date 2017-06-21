#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCReadDigital2/Z[=number:displayErrors]/DEV=number:deviceID
// number:channelNumber

extern "C" int ExecuteITCReadDigital2(ITCReadDigital2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);
  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  lITCChannelData[0].ChannelType = DIGITAL_INPUT;

  ITCDLL::ITC_AsyncIO(DeviceID, &lITCChannelData);

  // Store the result in V_Value
  if(int RetVal =
         SetOperationNumVar(RETURN_VARIABLE, (double) lITCChannelData[0].Value))
  {
    throw IgorException(RetVal);
  }

  END_OUTER_CATCH
}
