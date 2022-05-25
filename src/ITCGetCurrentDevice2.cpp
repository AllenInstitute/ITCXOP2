#include "ITC_StandardHeaders.h"

#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template: ITCGetCurrentDevice2/Z[=number:displayErrors]

extern "C" int
ExecuteITCGetCurrentDevice2(ITCGetCurrentDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  SetOperationReturn(RETURN_VARIABLE, DeviceIDClass::INVALID_DEVICE_ID);

  DeviceIDHelper DeviceID;

  SetOperationReturn(RETURN_VARIABLE, (double) DeviceID.getDeviceID());

  END_OUTER_CATCH
}
