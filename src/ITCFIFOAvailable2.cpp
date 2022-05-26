#include "ITC_StandardHeaders.h"
#include "ChannelSelection.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template: ITCFIFOAvailable2/Z[=number:displayErrors]
// /DEV=number:deviceID/CHN=number:channelTypeNumeric/CHS=string:channelTypeString
// number:channelNumber

extern "C" int ExecuteITCFIFOAvailable2(ITCFIFOAvailable2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  SetOperationReturn(RETURN_VARIABLE, -1);

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Get the FIFO available
  std::vector<ITCChannelDataEx> channelSelection(1);
  channelSelection[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  channelSelection[0].ChannelType =
      (unsigned short) GetChannelTypeFromParameters(p);
  channelSelection[0].Command = READ_FIFO_INFO;

  ITCDLL::ITC_GetDataAvailable(DeviceID, &channelSelection);

  SetOperationReturn(RETURN_VARIABLE, channelSelection[0].Value);

  END_OUTER_CATCH
}
