#include "ITC_StandardHeaders.h"

// Operation template: ITCWriteDigital2/Z[=number:displayErrors]
// /DEV=number:deviceID number:channelNumber, number:value

extern "C" int ExecuteITCWriteDigital2(ITCWriteDigital2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);
  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  lITCChannelData[0].ChannelType = DIGITAL_OUTPUT;

  if(!p->valueEncountered)
  {
    throw IgorException(OPAND_MISMATCH);
  }

  // Lock to uint16_t range (output is a 16-bit value)
  lITCChannelData[0].Value = (DWORD) lockToIntegerRange<uint16_t>(p->value);

  ITCDLL::ITC_AsyncIO(DeviceID, &lITCChannelData);

  END_OUTER_CATCH
}
