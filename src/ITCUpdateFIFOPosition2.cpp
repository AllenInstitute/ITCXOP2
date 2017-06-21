#include "ITC_StandardHeaders.h"

// Operation template:
// ITCUpdateFIFOPosition2/Z[=number:displayErrors]/DEV=number:deviceID/L[=number:lastFIFO]
// /R[=number:resetFIFO]
// /CHN=number:channelTypeNumeric/CHS=string:channelTypeString
// number:channelNumber, number:position

namespace
{

bool ReadLFlag(ITCUpdateFIFOPosition2RuntimeParamsPtr p)
{
  if(p->LFlagEncountered)
  {
    if(p->LFlagParamsSet[0])
    {
      // Optional parameter: p->lastFIFO
      return lockToIntegerRange<bool>(p->lastFIFO);
    }

    // Case of /L
    return true;
  }

  // /L flag not encountered.
  return false;
}

bool ReadRFlag(ITCUpdateFIFOPosition2RuntimeParamsPtr p)
{
  if(p->RFlagEncountered)
  {
    if(p->RFlagParamsSet[0])
    {
      // Optional parameter: p->resetFIFO
      return lockToIntegerRange<bool>(p->resetFIFO);
    }

    // Case of /R
    return true;
  }

  // /R flag not encountered.
  return false;
}

} // anonymous namespace

extern "C" int
ExecuteITCUpdateFIFOPosition2(ITCUpdateFIFOPosition2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);
  lITCChannelData[0].ChannelType =
      (unsigned short) GetChannelTypeFromParameters(p);
  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);

  if(ReadLFlag(p))
  {
    lITCChannelData[0].Command |= LAST_FIFO_COMMAND_EX;
  }

  if(ReadRFlag(p))
  {
    lITCChannelData[0].Command |= RESET_FIFO_COMMAND_EX;
  }

  // Check for the required position operand
  if(!p->positionEncountered)
  {
    IgorException(OPAND_MISMATCH);
  }

  // Special case of -1
  if(p->position == -1)
  {
    lITCChannelData[0].Value = (DWORD) -1;
  }
  else
  {
    lITCChannelData[0].Value = lockToIntegerRange<DWORD>(p->position);
  }

  ITCDLL::ITC_UpdateFIFOPosition(DeviceID, lITCChannelData);

  END_OUTER_CATCH
}
