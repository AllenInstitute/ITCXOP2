#include "ITC_StandardHeaders.h"
#include "HelperFunctions_DAC.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCSetDAC2/Z[=number:displayErrors]/DEV=number:deviceID/V[=number:voltageOn]/C[=number:calibrateOn]
// number:channelNumber, number:voltageOrDACUnits

namespace
{

DWORD getVoltage(ITCSetDAC2RuntimeParamsPtr p)
{
  // Check for required voltage parameter
  if(!p->voltageOrDACUnitsEncountered)
  {
    throw IgorException(OPAND_MISMATCH);
  }

  bool convertVoltage = ReadConvertVoltageFlag(p);

  if(convertVoltage)
  {
    // If value is NaN or inf, return an appropriate error.
    if(std::isnan(p->voltageOrDACUnits) || std::isinf(p->voltageOrDACUnits))
    {
      throw IgorException(kDoesNotSupportNaNorINF);
    }

    // If value is out of range, return an appropriate error.
    if((p->voltageOrDACUnits > 10.2396875) || (p->voltageOrDACUnits < -10.24))
    {
      throw IgorException(kParameterOutOfRange);
    }

    // Convert from volts to DAC units
    return (DWORD) lockToIntegerRange<short>(p->voltageOrDACUnits * ANALOGVOLT);
  }
  else
  {
    // Value is in DAC units already.
    return (DWORD) lockToIntegerRange<short>(p->voltageOrDACUnits);
  }
}

} // anonymous namespace

extern "C" int ExecuteITCSetDAC2(ITCSetDAC2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);
  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  lITCChannelData[0].ChannelType = H2D;

  lITCChannelData[0].Value = getVoltage(p);

  bool calibrate = ReadCalibrateFlag(p);
  if(!calibrate)
  {
    lITCChannelData[0].Command = DISABLE_CALIBRATION_EX;
  }

  ITCDLL::ITC_AsyncIO(DeviceID, &lITCChannelData);

  END_OUTER_CATCH
}
