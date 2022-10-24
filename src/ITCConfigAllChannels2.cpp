#include "ITC_StandardHeaders.h"
#include <algorithm>
#include "HelperFunctions.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

namespace
{

std::vector<ITCChannelInfo>
readConfigWaves(ITCConfigAllChannels2RuntimeParamsPtr p)
{
  // The config wave is a wave with at least 5 rows:
  // [0] - Channel Type
  // [1] - Channel Number
  // [2] - Sampling Interval (us)
  // [3] - Decimation mode
  // [4] - Offset into data wave
  std::vector<size_t> configDimSizes = checkWave_2D(p->config, NT_FP64, 5);

  // Each column in the config wave refers to one channel
  const size_t numChannels = configDimSizes[COLUMNS];

  // Each column in the data wave refers to the same channel as the one in the
  // config wave
  // Require a 16-bit wave
  std::vector<size_t> dataDimSizes = checkWave_2D(p->data, NT_I16, 1);

  // Must have one column per channel in the data wave
  if(dataDimSizes[COLUMNS] != numChannels)
  {
    throw IgorException(INCOMPATIBLE_DIMENSIONING);
  }

  // Make a vector to hold the data
  ITCChannelInfo loc;
  ZeroMemory(&loc, sizeof(ITCChannelInfo));
  std::vector<ITCChannelInfo> ChannelInfo(numChannels, loc);

  double *configPtr = (double *) WaveData(p->config);
  int16_t *dataPtr  = (int16_t *) WaveData(p->data);

  const DWORD NumberOfPoints = To<DWORD>(dataDimSizes[ROWS]);

  // Fill the vector
  for(size_t currChannel = 0; currChannel < numChannels; currChannel++)
  {
    ChannelInfo[currChannel].ChannelType =
        (DWORD) configPtr[getOffset_2D(0, currChannel, configDimSizes)];
    ChannelInfo[currChannel].ChannelNumber =
        (DWORD) configPtr[getOffset_2D(1, currChannel, configDimSizes)];
    ChannelInfo[currChannel].SamplingIntervalFlag =
        USE_TIME | US_SCALE; // | ADJUST_RATE;
    ChannelInfo[currChannel].SamplingRate =
        (DWORD) configPtr[getOffset_2D(2, currChannel, configDimSizes)];
    ChannelInfo[currChannel].ExternalDecimation =
        (DWORD) configPtr[getOffset_2D(3, currChannel, configDimSizes)];

    const auto offset = lockToIntegerRange<DWORD>(
        configPtr[getOffset_2D(4, currChannel, configDimSizes)]);

    if(offset >= NumberOfPoints)
    {
      throw IgorException(INCOMPATIBLE_DIMENSIONING);
    }

    ChannelInfo[currChannel].FIFOPointer =
        (void *) (dataPtr + getOffset_2D(0, currChannel, dataDimSizes) +
                  offset);
    ChannelInfo[currChannel].FIFONumberOfPoints = NumberOfPoints - offset;
  }

  // Return the vector
  return ChannelInfo;
}

} // anonymous namespace

// Operation template:
// ITCConfigAllChannels2/Z[=number:displayErrors]/DEV=number:deviceID
// wave:config, wave:data
extern "C" int
ExecuteITCConfigAllChannels2(ITCConfigAllChannels2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Generate the set-up vector
  std::vector<ITCChannelInfo> ChannelInfo = readConfigWaves(p);

  // The current ITC XOP takes care of reset, set and upload
  ITCDLL::ITC_ResetChannels(DeviceID);
  ITCDLL::ITC_SetChannels(DeviceID, &ChannelInfo);
  ITCDLL::ITC_UpdateChannels(DeviceID);

  END_OUTER_CATCH
}
