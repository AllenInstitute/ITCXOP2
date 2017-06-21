#include "ITC_StandardHeaders.h"
#include "ChannelSelection.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCFIFOAvailableAll2/Z[=number:displayErrors]/O/FREE/DEV=number:deviceID
// wave:{channelSelection,real}, DataFolderAndName:{dest,real}

void writeChannelValueWave(
    ITCFIFOAvailableAll2RuntimeParamsPtr p,
    const std::vector<ITCChannelDataEx> &ChannelSelection)
{

  // ResultWave: 64 - bit floating point Wave with at least 4 entries :
  const int numRows = 4;
  const int numCols = static_cast<int>(ChannelSelection.size());

  std::vector<std::string> RowNames{
      "ChannelType",   // [0]  : ChannelType - integer value of channel type
                       // representation(see also Channel Type)
      "ChannelNumber", // [1]  : ChannelNumber - integer value of channel
                       // number(see also Channel Number)
      "Value",         // [2]  : Value - Number of points available
      "",              // [3]  : Reserved
  };
  std::vector<std::string> ColNames;

  // Destination wave handle
  waveHndl h = nullptr;

  const int dataType = NT_FP64;
  MakeDestWave(p, &h, dataType, numRows, numCols, RowNames, ColNames);

// Copy the data across
#define STORE_CHANNELDATAEX_FN(Column, Field)                                  \
  storeDoubleData_waveRow<ITCChannelDataEx>(                                   \
      ChannelSelection.begin(), ChannelSelection.end(), h,                     \
      [](const ITCChannelDataEx &I) -> double { return (double) I.Field; },    \
      Column)

  STORE_CHANNELDATAEX_FN(0, ChannelType);
  STORE_CHANNELDATAEX_FN(1, ChannelNumber);
  STORE_CHANNELDATAEX_FN(2, Value);

#undef STORE_CHANNELDATAEX_FN

  FinishDestWave(p, h);
}

extern "C" int
ExecuteITCFIFOAvailableAll2(ITCFIFOAvailableAll2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Acquire a DeviceID
  DeviceIDHelper DeviceID(p);

  // Read the selection wave
  std::vector<ITCChannelDataEx> ChannelSelection =
      makeChannelSelectionVec<ITCChannelDataEx>(p, DeviceID);

  // Set the operation on all elements
  for(auto &s : ChannelSelection)
  {
    s.Command = READ_FIFO_INFO;
  }

  // Get the channel information
  ITCDLL::ITC_GetDataAvailable(DeviceID, &ChannelSelection);

  // Copy to the output wave
  writeChannelValueWave(p, ChannelSelection);

  END_OUTER_CATCH
}
