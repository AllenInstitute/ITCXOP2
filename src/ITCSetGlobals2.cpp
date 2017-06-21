#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCSetGlobals2/Z[=number:displayErrors]/D[=number:debugOutput]
extern "C" int ExecuteITCSetGlobals2(ITCSetGlobals2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  ErrorDisplayClass::SetGlobalZFlag(p);
  ReadGlobalDebugFlag(p);
  END_OUTER_CATCH
}
