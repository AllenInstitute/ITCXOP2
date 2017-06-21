#include "ITC_StandardHeaders.h"
// Operation template: ITCGetCurrentDevice2/Z[=number:displayErrors]

extern "C" int
ExecuteITCGetCurrentDevice2(ITCGetCurrentDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  DeviceIDHelper DeviceID;

  // Store the result in V_Value
  if(int RetVal =
         SetOperationNumVar(RETURN_VARIABLE, (double) DeviceID.getDeviceID()))
  {
    throw IgorException(RetVal);
  }

  END_OUTER_CATCH
}
