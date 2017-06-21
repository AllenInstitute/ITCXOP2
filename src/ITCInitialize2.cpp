#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template: ITCInitialize2/Z[=number:displayErrors]
// /DEV=number:deviceID/M=number:mode/U=string:file/F=number:fFunc/D=number:dFunc/H=number:hFunc/R=number:rFunc

extern "C" int ExecuteITCInitialize2(ITCInitialize2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // The /F flag requires ITC18/ITC18USB/ITC1600
  // The /D, /H and /F flags require ITC1600

  // Read the device type
  ITCDeviceTypeEnum DeviceType;
  ITCDLL::ITC_GetDeviceType(DeviceID, &DeviceType, NULL);

  HWFunction lHWFunction = {};

  // Special HWFunctions for ITC18 and ITC1600
  bool useSpecialHWFunction           = false;
  ITC18_Special_HWFunction lITC18     = {};
  ITC1600_Special_HWFunction lITC1600 = {};

  // Flag parameters.
  if(p->MFlagEncountered)
  {
    // Parameter: p->mode
    if(p->mode == -1)
    {
      lHWFunction.Mode = (DWORD) -1;
    }
    else
    {
      lHWFunction.Mode = lockToIntegerRange<DWORD>(p->mode);
    }
  }
  else
  {
    // /M flag not encountered
    lHWFunction.Mode = 0;
  }

  std::string file;
  if(p->UFlagEncountered)
  {
    // Parameter: p->file
    file                 = getStringFromHandle(p->file);
    lHWFunction.U2F_File = const_cast<char *>(file.c_str());
  }
  else
  {
    lHWFunction.U2F_File = nullptr;
  }

  if(p->FFlagEncountered)
  {
    // Parameter: p->fFunc

    // Check device type
    if((DeviceType != ITCDeviceTypeEnum::ITC18) &&
       (DeviceType != ITCDeviceTypeEnum::ITC18USB) &&
       (DeviceType != ITCDeviceTypeEnum::ITC1600))
    {
      // /F flag requires ITC18, ITC18USB or ITC1600
      throw IgorException(F_FLAG_REQUIRES_ITC18_ITC18USB_ITC1600);
    }

    useSpecialHWFunction = true;
    lITC18.Function      = lockToIntegerRange<DWORD>(p->fFunc);
    lITC1600.Function    = lockToIntegerRange<DWORD>(p->fFunc);
  }

  if(p->DFlagEncountered)
  {
    // Parameter: p->dFunc

    // Check device type
    if(DeviceType != ITCDeviceTypeEnum::ITC1600)
    {
      // /D flag requires ITC1600
      throw IgorException(D_FLAG_REQUIRES_ITC1600);
    }

    useSpecialHWFunction = true;
    lITC1600.DSPType     = lockToIntegerRange<DWORD>(p->dFunc);
  }

  if(p->HFlagEncountered)
  {
    // Parameter: p->hFunc

    // Check device type
    if(DeviceType != ITCDeviceTypeEnum::ITC1600)
    {
      // /H flag requires ITC1600
      throw IgorException(H_FLAG_REQUIRES_ITC1600);
    }

    useSpecialHWFunction = true;
    lITC1600.HOSTType    = lockToIntegerRange<DWORD>(p->hFunc);
  }

  if(p->RFlagEncountered)
  {
    // Parameter: p->rFunc

    // Check device type
    if(DeviceType != ITCDeviceTypeEnum::ITC1600)
    {
      throw IgorException(R_FLAG_REQUIRES_ITC1600);
    }

    useSpecialHWFunction = true;
    lITC1600.RACKType    = lockToIntegerRange<DWORD>(p->rFunc);
  }

  if(useSpecialHWFunction)
  {
    switch(DeviceType)
    {
    case ITCDeviceTypeEnum::ITC18:
    case ITCDeviceTypeEnum::ITC18USB:
      lHWFunction.SpecialFunction       = &lITC18;
      lHWFunction.SizeOfSpecialFunction = sizeof(lITC18);
      break;
    case ITCDeviceTypeEnum::ITC1600:
      lHWFunction.SpecialFunction       = &lITC1600;
      lHWFunction.SizeOfSpecialFunction = sizeof(lITC1600);
      break;
    default:
      // A special HW function has been requested, but the device doesn't
      // support it.
      // Should never reach here, since we checked device types above.
      throw IgorException(INCOMPATIBLE_FLAGS);
    }
  }

  ITCDLL::ITC_InitDevice(DeviceID, &lHWFunction);

  END_OUTER_CATCH
}
