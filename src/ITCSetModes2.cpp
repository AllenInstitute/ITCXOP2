#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCSetModes2/Z[=number:displayErrors]/DEV=number:deviceID/I=number:DigitalInputMode/E=number:ExternalTriggerMode/T=number:ExternalTrigger/C[=number:EnableExternalClock]/D=number:DACShiftValue/R=number:InputRange/P=number:TriggerOutPosition/O[=number:OutputEnable]/F[=number:ResetFIFOFlag]/L=number:ControlLight/S=number:SamplingInterval
extern "C" int ExecuteITCSetModes2(ITCSetModes2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  DeviceIDHelper DeviceID(p);

  ITCPublicConfig lConfig;

  lConfig.DigitalInputMode    = -1;
  lConfig.ExternalTriggerMode = -1;
  lConfig.ExternalTrigger     = -1;
  lConfig.EnableExternalClock = -1;
  lConfig.DACShiftValue       = -1;
  lConfig.InputRange          = -1;
  lConfig.TriggerOutPosition  = -1;
  lConfig.OutputEnable        = -1;
  lConfig.SequenceLength      = -1;
  lConfig.Sequence            = NULL;
  lConfig.SequenceLengthIn    = -1;
  lConfig.SequenceIn          = NULL;
  lConfig.ResetFIFOFlag       = -1;
  lConfig.ControlLight        = -1;
  lConfig.SamplingInterval    = -1;

  if(p->IFlagEncountered)
  {
    // Parameter: p->DigitalInputMode
    lConfig.DigitalInputMode = lockToIntegerRange<DWORD>(p->DigitalInputMode);
  }

  if(p->EFlagEncountered)
  {
    // Parameter: p->ExternalTriggerMode
    lConfig.ExternalTriggerMode =
        lockToIntegerRange<DWORD>(p->ExternalTriggerMode);
  }

  if(p->TFlagEncountered)
  {
    // Parameter: p->ExternalTrigger
    lConfig.ExternalTrigger = lockToIntegerRange<DWORD>(p->ExternalTrigger);
  }

  if(p->CFlagEncountered)
  {
    if(p->CFlagParamsSet[0])
    {
      // Optional parameter: p->EnableExternalClock
      lConfig.EnableExternalClock =
          lockToIntegerRange<DWORD>(p->EnableExternalClock);
    }
    else
    {
      lConfig.EnableExternalClock = 1;
    }
  }

  if(p->DFlagEncountered)
  {
    // Parameter: p->DACShiftValue
    lConfig.DACShiftValue = lockToIntegerRange<DWORD>(p->DACShiftValue);
  }

  if(p->RFlagEncountered)
  {
    // Parameter: p->InputRange
    lConfig.InputRange = lockToIntegerRange<DWORD>(p->InputRange);
  }

  if(p->PFlagEncountered)
  {
    // Parameter: p->TriggerOutPosition
    lConfig.TriggerOutPosition =
        lockToIntegerRange<DWORD>(p->TriggerOutPosition);
  }

  if(p->OFlagEncountered)
  {
    if(p->OFlagParamsSet[0])
    {
      // Optional parameter: p->OutputEnable
      lConfig.OutputEnable = lockToIntegerRange<DWORD>(p->OutputEnable);
    }
    else
    {
      lConfig.OutputEnable = 1;
    }
  }

  if(p->FFlagEncountered)
  {
    if(p->FFlagParamsSet[0])
    {
      // Optional parameter: p->ResetFIFOFlag
      lConfig.ResetFIFOFlag = lockToIntegerRange<DWORD>(p->ResetFIFOFlag);
    }
    else
    {
      lConfig.ResetFIFOFlag = 1;
    }
  }

  if(p->LFlagEncountered)
  {
    // Parameter: p->ControlLight
    lConfig.ControlLight = lockToIntegerRange<DWORD>(p->ControlLight);
  }

  if(p->SFlagEncountered)
  {
    // Parameter: p->SamplingInterval
    lConfig.SamplingInterval = p->SamplingInterval;
  }

  ITCDLL::ITC_ConfigDevice(DeviceID, &lConfig);

  END_OUTER_CATCH
}
