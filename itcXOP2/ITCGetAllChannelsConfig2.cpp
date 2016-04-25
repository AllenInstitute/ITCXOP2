#include "ITC_StandardHeaders.h"
#include "ChannelSelection.h"

// Operation template:
// ITCGetAllChannelsConfig2/Z[=number:displayErrors]/O/FREE/DEV=number:deviceID
// wave:{channelSelection,real}, DataFolderAndName:{dest,real}

extern "C" int
ExecuteITCGetAllChannelsConfig2(ITCGetAllChannelsConfig2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Acquire a DeviceID
  DeviceIDHelper DeviceID(p);

  // Read the selection wave
  std::vector<ITCChannelInfo> ChannelSelection =
      makeChannelSelectionVec<ITCChannelInfo>(p, DeviceID);

  // Get the channel information
  ITCDLL::ITC_GetChannels(DeviceID, &ChannelSelection);

  // Copy to the output wave
  writeChannelConfigWave(p, ChannelSelection);

  END_OUTER_CATCH
}
