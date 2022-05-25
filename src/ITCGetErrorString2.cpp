#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCGetErrorString2/Z[=number:displayErrors]/DEV[=number:deviceID] /X
// number:errorCode

extern "C" int ExecuteITCGetErrorString2(ITCGetErrorString2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  SetOperationReturn(ERROR_RETURN_VARIABLE, "");

  if(!p->errorCodeEncountered)
  {
    // Parameter: p->errorCode
    throw IgorException(OH_EXPECTED_NUMBER);
  }

  HANDLE deviceHandle;
  DWORD status = lockToIntegerRange<DWORD>(p->errorCode);

  // Since ITC_GetStatusText can be called with or without a device handle,
  // there are 3 ways of calling this function:
  // 1) No flags -- use nullptr as device handle
  // 2) /DEV -- use thread device ID
  // 3) /DEV=k -- use device ID k
  if(p->DEVFlagEncountered)
  {
    if(p->DEVFlagParamsSet[0])
    {
      // /DEV=k
      DeviceIDHelper DeviceID(p);
      deviceHandle = DeviceID.getHandle();
    }
    else
    {
      // /DEV
      DeviceIDHelper DeviceID;
      deviceHandle = DeviceID.getHandle();
    }
  }
  else
  {
    // No /DEV flag
    deviceHandle = nullptr;
  }

  const DWORD messageLen       = 256;
  char StatusText[messageLen]  = {};
  char AnalyzeText[messageLen] = {};

  // Get the standard error message
  ITCDLL::ITC_GetStatusText(deviceHandle, status, StatusText, messageLen);
  std::string Message(StatusText);

  // /X flag means "Get additional information"
  if(p->XFlagEncountered)
  {
    ITCDLL::ITC_AnalyzeError(status, AnalyzeText, messageLen);
    Message += ";" + std::string(AnalyzeText);
  }

  SetOperationReturn(ERROR_RETURN_VARIABLE, Message);

  END_OUTER_CATCH
}
