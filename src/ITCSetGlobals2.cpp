#include "ITC_StandardHeaders.h"
#include "Logging.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCSetGlobals2/Z[=number:displayErrors]/D[=number:debugOutput]/LTS=string:logTemplateString
extern "C" int ExecuteITCSetGlobals2(ITCSetGlobals2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH
  ErrorDisplayClass::SetGlobalZFlag(p);
  ReadGlobalDebugFlag(p);

  if(p->LTSFlagEncountered)
  {
    if(!p->LTSFlagParamsSet[0])
    {
      throw IgorException(GENERAL_BAD_VIBS);
    }

    auto templateStr = getStringFromHandle(p->logTemplateString);
    SetLoggingTemplate(templateStr);
  }

  END_OUTER_CATCH
}
