#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCGetDevices2/Z[=number:displayErrors]/DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString

extern "C" int ExecuteITCGetDevices2(ITCGetDevices2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  // Before we call the function, set V_Value to the error value.
  SetOperationReturn(RETURN_VARIABLE, -1);

  // Get the device type from the DTN or DTS flag
  ITCDeviceTypeEnum DeviceType = GetDeviceTypeFromParameters(p);

  // Get the device list
  DWORD numDevices;
  ITCDLL::ITC_Devices(DeviceType, &numDevices);

  SetOperationReturn(RETURN_VARIABLE, (double) numDevices);

  END_OUTER_CATCH
}
