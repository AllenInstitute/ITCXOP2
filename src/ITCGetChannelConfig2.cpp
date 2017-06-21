#include "ITC_StandardHeaders.h"

// Operation template: ITCGetChannelConfig2/Z[=number:displayErrors]
// /DEV=number:deviceID/CHN=number:channelTypeNumeric/CHS=string:channelTypeString/O/FREE
// number:channelNumber, DataFolderAndName:{dest,real}

extern "C" int
ExecuteITCGetChannelConfig2(ITCGetChannelConfig2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelInfo> ChannelSelection(1);
  ChannelSelection[0].ChannelNumber = GetChannelNumberFromParameters<DWORD>(p);
  ChannelSelection[0].ChannelType   = (DWORD) GetChannelTypeFromParameters(p);

  // Get the channel information
  ITCDLL::ITC_GetChannels(DeviceID, &ChannelSelection);

  // Copy to the output wave
  writeChannelConfigWave(p, ChannelSelection);

  END_OUTER_CATCH
}
