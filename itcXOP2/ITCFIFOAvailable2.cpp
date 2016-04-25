#include "ITC_StandardHeaders.h"
#include "ChannelSelection.h"

// Operation template: ITCFIFOAvailable2/Z[=number:displayErrors]
// /DEV=number:deviceID/CHN=number:channelTypeNumeric/CHS=string:channelTypeString
// number:channelNumber

extern "C" int ExecuteITCFIFOAvailable2(ITCFIFOAvailable2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

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

  // Store the result in V_Value
  if(int RetVal = SetOperationNumVar(RETURN_VARIABLE,
                                     (double) channelSelection[0].Value))
  {
    throw IgorException(RetVal);
  }

  END_OUTER_CATCH
}
