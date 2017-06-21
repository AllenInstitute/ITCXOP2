#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCStartAcq2/Z[=number:displayErrors]/DEV=number:deviceID/R[=number:resetFIFOs]/F[=number:fastPointerUpdate]/OE=number:outputEnable/EXT=number:externalTrigger/STPO=number:stopOnOverflow/STPU=number:stopOnUnderrun/STRT=number:startTime/STOP=number:stopTime

namespace
{

DWORD ReadRFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->RFlagEncountered)
  {
    if(p->RFlagParamsSet[0])
    {
      // Optional parameter: p->resetFIFOs
      // Case of /R=n
      // Only supporting /R=0 and /R=1
      return lockToIntegerRange<bool>(p->resetFIFOs);
    }

    // Case of /R
    // If the flag is specified without a value, then reset FIFOs
    return 1;
  }

  // Flag not encountered.
  // Default value is 0 (do not reset FIFOs)
  return 0;
}

DWORD ReadFFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->FFlagEncountered)
  {
    if(p->FFlagParamsSet[0])
    {
      // Optional parameter: p->fastPointerUpdate
      // Case of /F=n

      // Special case of -1
      if(p->fastPointerUpdate == -1)
      {
        return (DWORD) -1;
      }

      return lockToIntegerRange<DWORD>(p->fastPointerUpdate);
    }

    // Case of /F
    // If the flag is specified without a value, then use FastPointerUpdate
    return FastPointerUpdate;
  }

  // Flag not encountered.
  // Default value is -1 (do not change)
  return (DWORD) -1;
}

DWORD ReadOEFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->OEFlagEncountered)
  {
    // Parameter: p->outputEnable

    // Special case of -1
    if(p->outputEnable == -1)
    {
      return (DWORD) -1;
    }

    return lockToIntegerRange<DWORD>(p->outputEnable);
  }

  // Flag not encountered.
  // Default value is -1 (do not change)
  return (DWORD) -1;
}

DWORD ReadEXTFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->EXTFlagEncountered)
  {
    // Parameter: p->externalTrigger

    // Special case of -1
    if(p->externalTrigger == -1)
    {
      return (DWORD) -1;
    }
    return lockToIntegerRange<DWORD>(p->externalTrigger);
  }

  // Flag not encountered.
  // Default value is -1 (do not change)
  return (DWORD) -1;
}

DWORD ReadSTPOFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->STPOFlagEncountered)
  {
    // Parameter: p->stopOnOverflow

    // Special case of -1
    if(p->stopOnOverflow == -1)
    {
      return (DWORD) -1;
    }
    return lockToIntegerRange<DWORD>(p->stopOnOverflow);
  }

  // Flag not encountered.
  // Default value is -1 (do not change)
  return (DWORD) -1;
}

DWORD ReadSTPUFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->STPUFlagEncountered)
  {
    // Parameter: p->stopOnUnderrun

    // Special case of -1
    if(p->stopOnUnderrun == -1)
    {
      return (DWORD) -1;
    }

    return lockToIntegerRange<DWORD>(p->stopOnUnderrun);
  }

  // Flag not encountered.
  // Default value is -1 (do not change)
  return (DWORD) -1;
}

double ReadSTRTFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->STRTFlagEncountered)
  {
    // If value is NaN or inf, return an appropriate error.
    if(std::isnan(p->startTime) || std::isinf(p->startTime))
    {
      throw IgorException(kDoesNotSupportNaNorINF);
    }

    return p->startTime;
  }

  // Default value is 0 (start immediately)
  return 0;
}

double ReadSTOPFlag(ITCStartAcq2RuntimeParamsPtr p)
{
  if(p->STOPFlagEncountered)
  {
    // If value is NaN or inf, return an appropriate error.
    if(std::isnan(p->stopTime) || std::isinf(p->stopTime))
    {
      throw IgorException(kDoesNotSupportNaNorINF);
    }

    return p->stopTime;
  }

  // Default value is 0 (start immediately)
  return 0;
}

} // anonymous namespace

extern "C" int ExecuteITCStartAcq2(ITCStartAcq2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Read the flags
  ITCStartInfo lITCStartInfo    = {};
  lITCStartInfo.ResetFIFOs      = ReadRFlag(p);
  lITCStartInfo.RunningOption   = ReadFFlag(p);
  lITCStartInfo.OutputEnable    = ReadOEFlag(p);
  lITCStartInfo.ExternalTrigger = ReadEXTFlag(p);
  lITCStartInfo.StopOnOverflow  = ReadSTPOFlag(p);
  lITCStartInfo.StopOnUnderrun  = ReadSTPUFlag(p);
  lITCStartInfo.StartTime       = ReadSTRTFlag(p);
  lITCStartInfo.StopTime        = ReadSTOPFlag(p);

  // ITCDLL::ITC_Start() can be called in 2 ways:
  // 1) ITCDLL::ITC_Start(DeviceID, &lITCStartInfo);
  //	--> Upload new settings
  // 2) ITCDLL::ITC_Start(DeviceID, nulltpr);
  //	--> Do not upload new settings.  This is faster than uploading new
  // settings
  //
  // If the flags are set such that no settings will be changed, then we can
  // use the faster version
  //
  // The requirements for this are that all settings are set to "no action" or
  // "do not change"
  // This is the case when:
  // ResetFIFOs set to 0
  // RunningOption set to -1
  // OutputEnable set to -1
  // ExternalTrigger set to -1
  // StopOnOverflow set to -1
  // StopOnUnderrun set to -1
  // StartTime set to 0
  // StopTime set to 0
  if(lITCStartInfo.ResetFIFOs == 0 &&
     lITCStartInfo.RunningOption == (DWORD) -1 &&
     lITCStartInfo.OutputEnable == (DWORD) -1 &&
     lITCStartInfo.ExternalTrigger == (DWORD) -1 &&
     lITCStartInfo.StopOnOverflow == (DWORD) -1 &&
     lITCStartInfo.StopOnUnderrun == (DWORD) -1 &&
     lITCStartInfo.StartTime == 0 && lITCStartInfo.StopTime == 0)
  {
    // Flags set such that update is not necessary
    // Use the "nullptr" version
    ITCDLL::ITC_Start(DeviceID, nullptr);
  }
  else
  {
    // Update configuration on start
    ITCDLL::ITC_Start(DeviceID, &lITCStartInfo);
  }

  END_OUTER_CATCH
}
