#pragma once
#include "ITC_StandardHeaders.h"
#include <algorithm>

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// @brief Get a selector for all channels
template <typename T>
std::vector<T> GetAllChannelSelection(const DeviceIDHelper &DeviceID)
{
  // Read the device type
  ITCDeviceTypeEnum DeviceType;
  ITCDLL::ITC_GetDeviceType(DeviceID, &DeviceType, NULL);

  size_t numInputChannels  = 0;
  size_t numOutputChannels = 0;
  // Work out which device we've got
  switch(DeviceType)
  {
  case ITCDeviceTypeEnum::ITC16:
    numInputChannels  = ITC16_NUMBEROFINPUTS;
    numOutputChannels = ITC16_NUMBEROFOUTPUTS;
    break;

  case ITCDeviceTypeEnum::ITC18:
    numInputChannels  = ITC18_NUMBEROFINPUTS;
    numOutputChannels = ITC18_NUMBEROFOUTPUTS;
    break;

  case ITCDeviceTypeEnum::ITC1600:
    numInputChannels  = ITC1600_NUMBEROFINPUTS;
    numOutputChannels = ITC1600_NUMBEROFOUTPUTS;
    break;

  case ITCDeviceTypeEnum::ITC00:
    numInputChannels  = ITC00_NUMBEROFINPUTS;
    numOutputChannels = ITC00_NUMBEROFOUTPUTS;
    break;

  case ITCDeviceTypeEnum::ITC16USB:
    numInputChannels  = USB16_NUMBEROFINPUTS;
    numOutputChannels = USB16_NUMBEROFOUTPUTS;
    break;

  case ITCDeviceTypeEnum::ITC18USB:
    numInputChannels  = USB18_NUMBEROFINPUTS;
    numOutputChannels = USB18_NUMBEROFOUTPUTS;
    break;

  default:
    // TODO: Custom error code
    throw IgorException();
  }

  // Fill in the vector.
  // Assuming (based on documentation):
  // - All input channels are sequentially numbered, starting from 0 to
  // NUMBEROFINPUTS-1
  // - All output channels are sequentially numbered, starting from 0 to
  // NUMBEROFOUTPUTS-1

  const size_t totalNumChannels = numInputChannels + numOutputChannels;

  // Make a vector
  std::vector<T> channelSelection(totalNumChannels);

  // Fill in the inputs
  for(size_t k = 0; k < numInputChannels; k++)
  {
    channelSelection[k].ChannelNumber =
        static_cast<decltype(T::ChannelNumber)>(k);
    channelSelection[k].ChannelType = INPUT_GROUP;
  }

  // And fill in the outputs
  for(size_t k = 0; k < numOutputChannels; k++)
  {
    channelSelection[k + numInputChannels].ChannelNumber =
        static_cast<decltype(T::ChannelNumber)>(k);
    channelSelection[k + numInputChannels].ChannelType = OUTPUT_GROUP;
  }

  return channelSelection;
}

template <typename T>
std::vector<T> GetChannelSelection(waveHndl waveH)
{
  // Assume that wave has already been checked for NULL in the calling function

  // Check the wave type
  // Allow:
  // FP64, FP32, I64, U64, I32, U32, I16, U16, I8, U8
  const int currWaveType = WaveType(waveH);
  if(currWaveType != NT_FP64 && currWaveType != NT_FP32 &&
     currWaveType != NT_I64 && currWaveType != (NT_I64 | NT_UNSIGNED) &&
     currWaveType != NT_I32 && currWaveType != (NT_I32 | NT_UNSIGNED) &&
     currWaveType != NT_I16 && currWaveType != (NT_I16 | NT_UNSIGNED) &&
     currWaveType != NT_I8 && currWaveType != (NT_I8 | NT_UNSIGNED))
  {
    throw IgorException(NT_INCOMPATIBLE);
  }

  int numDimensions;
  CountInt dimensionSizes[MAX_DIMENSIONS + 1];
  if(int RetVal = MDGetWaveDimensions(waveH, &numDimensions, dimensionSizes))
  {
    throw IgorException(RetVal);
  }

  // Number of rows should be at least 2.
  // Channel Type and Channel Number
  if(dimensionSizes[ROWS] < 2)
  {
    throw IgorException(NEED_MIN_ROWS);
  }

  // Number of columns is the number of channels to read.

  // Number of layers must be 0 (2D wave), since the indexing assumes a 2D wave
  if(dimensionSizes[LAYERS] != 0)
  {
    throw IgorException(EXPECT_MATRIX);
  }

  // Make the selection vector.
  const size_t numColumns =
      std::max((size_t) dimensionSizes[COLUMNS], (size_t) 1);
  std::vector<T> channelSelection(numColumns);

  double *dp        = (double *) WaveData(waveH);
  float *fp         = (float *) WaveData(waveH);
  int64_t *int64p   = (int64_t *) WaveData(waveH);
  uint64_t *uint64p = (uint64_t *) WaveData(waveH);
  int32_t *int32p   = (int32_t *) WaveData(waveH);
  uint32_t *uint32p = (uint32_t *) WaveData(waveH);
  int16_t *int16p   = (int16_t *) WaveData(waveH);
  uint16_t *uint16p = (uint16_t *) WaveData(waveH);
  int8_t *int8p     = (int8_t *) WaveData(waveH);
  uint8_t *uint8p   = (uint8_t *) WaveData(waveH);

  const size_t numRows = dimensionSizes[ROWS];
  for(size_t k = 0; k < channelSelection.size(); k++)
  {
    switch(currWaveType)
    {
    case NT_FP64:
      channelSelection[k].ChannelType =
          lockToIntegerRange<decltype(T::ChannelType)>(dp[k * numRows]);
      channelSelection[k].ChannelNumber =
          lockToIntegerRange<decltype(T::ChannelNumber)>(dp[k * numRows + 1]);
      break;
    case NT_FP32:
      channelSelection[k].ChannelType =
          lockToIntegerRange<decltype(T::ChannelType)>(fp[k * numRows]);
      channelSelection[k].ChannelNumber =
          lockToIntegerRange<decltype(T::ChannelNumber)>(fp[k * numRows + 1]);
      break;
    case NT_I64:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(int64p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(int64p[k * numRows + 1]);
      break;
    case NT_I64 | NT_UNSIGNED:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(uint64p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(uint64p[k * numRows + 1]);
      break;
    case NT_I32:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(int32p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(int32p[k * numRows + 1]);
      break;
    case NT_I32 | NT_UNSIGNED:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(uint32p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(uint32p[k * numRows + 1]);
      break;
    case NT_I16:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(int16p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(int16p[k * numRows + 1]);
      break;
    case NT_I16 | NT_UNSIGNED:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(uint16p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(uint16p[k * numRows + 1]);
      break;
    case NT_I8:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(int8p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(int8p[k * numRows + 1]);
      break;
    case NT_I8 | NT_UNSIGNED:
      channelSelection[k].ChannelType =
          (decltype(T::ChannelType))(uint8p[k * numRows]);
      channelSelection[k].ChannelNumber =
          (decltype(T::ChannelNumber))(uint8p[k * numRows + 1]);
      break;
    default:
      throw IgorException(NT_INCOMPATIBLE);
    }
  }

  return channelSelection;
}

/// @brief Read an Igor wave into a channel selection
template <typename outT, typename T>
std::vector<outT> makeChannelSelectionVec(T p, const DeviceIDHelper &DeviceID)
{

  // Check that the config wave has been supplied
  // If the config wave isn't present, or, the wave is NULL, then select all
  // channels.
  if(!(p->channelSelectionEncountered) || p->channelSelection == nullptr)
  {
    return GetAllChannelSelection<outT>(DeviceID);
  }

  // Otherwise, read the wave values.
  waveHndl waveH = p->channelSelection;

  return GetChannelSelection<outT>(p->channelSelection);
}
