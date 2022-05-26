#include "ITC_StandardHeaders.h"
#include "HelperFunctions_ITC.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCReadADC2/Z[=number:displayErrors]/DEV=number:deviceID/V[=number:voltageOn]/C[=number:calibrateOn]
// number:channelNumber

extern "C" int ExecuteITCReadADC2(ITCReadADC2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  SetOperationReturn(RETURN_VARIABLE, DOUBLE_NAN);

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  std::vector<ITCChannelDataEx> lITCChannelData(1);

  lITCChannelData[0].ChannelNumber =
      GetChannelNumberFromParameters<unsigned short>(p);
  lITCChannelData[0].ChannelType = D2H;

  bool convertVoltage = ReadConvertVoltageFlag(p);

  bool calibrate = ReadCalibrateFlag(p);
  if(!calibrate)
  {
    lITCChannelData[0].Command = DISABLE_CALIBRATION_EX;
  }

  ITCDLL::ITC_AsyncIO(DeviceID, &lITCChannelData);

  double volts;
  if(convertVoltage == false)
    volts = (double) lITCChannelData[0].Value;
  else
  {
    if(lITCChannelData[0].ChannelNumber < 16)
      volts = (double) ((short) lITCChannelData[0].Value) / ANALOGVOLT;
    else
      volts = (double) ((short) lITCChannelData[0].Value) / SLOWANALOGVOLT;
  }

  SetOperationReturn(RETURN_VARIABLE, volts);

  END_OUTER_CATCH
}
