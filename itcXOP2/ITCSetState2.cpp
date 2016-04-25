#include "ITC_StandardHeaders.h"

// Operation template:
// ITCSetState2/Z[=number:displayErrors]/S=number:switchCommand/DEV=number:deviceID

extern "C" int ExecuteITCSetState2(ITCSetState2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  ITCStatus lStatus = {};

  if(p->SFlagEncountered)
  {
    lStatus.State =
        lockToIntegerRange<DWORD>(p->switchCommand) & SWITCH_OUTPUT_MASK;
  }

  // Set the state
  ITCDLL::ITC_SetState(DeviceID, &lStatus);

  END_OUTER_CATCH
}
