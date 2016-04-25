#include "ITC_StandardHeaders.h"
#include <algorithm>

namespace
{

std::vector<ITCChannelInfo>
readConfigWaves(ITCConfigAllChannels2RuntimeParamsPtr p)
{
  // The config wave is a wave with at least 4 rows.
  // At least 4 rows in the config wave:
  // [0] - Channel Type
  // [1] - Channel Number
  // [2] - Sampling Interval (us)
  // [3] - Decimation mode
  std::vector<size_t> configDimSizes = checkWave_2D(p->config, NT_FP64, 4);

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

    ChannelInfo[currChannel].FIFOPointer =
        (void *) (dataPtr + getOffset_2D(0, currChannel, dataDimSizes));
    ChannelInfo[currChannel].FIFONumberOfPoints = (DWORD) dataDimSizes[ROWS];
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
  ITCDLL::ITC_SetChannels(DeviceID, ChannelInfo);
  ITCDLL::ITC_UpdateChannels(DeviceID);

  END_OUTER_CATCH
}
