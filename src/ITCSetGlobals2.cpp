#include "ITC_StandardHeaders.h"

// Operation template:
// ITCSetGlobals2/Z[=number:displayErrors]/D[=number:debugOutput]
extern "C" int ExecuteITCSetGlobals2(ITCSetGlobals2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  ErrorDisplayClass::SetGlobalZFlag(p);
  ReadGlobalDebugFlag(p);
  END_OUTER_CATCH
}
