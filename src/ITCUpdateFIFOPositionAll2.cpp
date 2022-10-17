#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCUpdateFIFOPositionAll2/Z[=number:displayErrors]/DEV=number:deviceID
// wave:fifoPos

namespace
{

std::vector<ITCChannelDataEx>
readFifoPos(ITCUpdateFIFOPositionAll2RuntimeParamsPtr p)
{
  std::vector<size_t> dimSizes = checkWave_2D(p->fifoPos, NT_FP64, 3);

  // Make a vector to hold the data
  const size_t numChannels = dimSizes[COLUMNS];
  std::vector<ITCChannelDataEx> fifoPos(numChannels);

  double *configPtr = (double *) WaveData(p->fifoPos);

  for(size_t currChannel = 0; currChannel < numChannels; currChannel++)
  {
    fifoPos[currChannel].ChannelType =
        (unsigned short) configPtr[getOffset_2D(0, currChannel, dimSizes)];
    fifoPos[currChannel].ChannelNumber =
        (unsigned short) configPtr[getOffset_2D(1, currChannel, dimSizes)];
    fifoPos[currChannel].Value =
        (DWORD) configPtr[getOffset_2D(2, currChannel, dimSizes)];
  }

  return fifoPos;
}

} // anonymous namespace

extern "C" int
ExecuteITCUpdateFIFOPositionAll2(ITCUpdateFIFOPositionAll2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Acquire a DeviceID
  DeviceIDHelper DeviceID(p);

  // Read the set-up wave
  std::vector<ITCChannelDataEx> fifoPos = readFifoPos(p);

  // Call the function
  ITCDLL::ITC_UpdateFIFOPosition(DeviceID, &fifoPos);

  END_OUTER_CATCH
}
