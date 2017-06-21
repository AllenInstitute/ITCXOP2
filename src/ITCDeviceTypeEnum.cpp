#include "ITC_StandardHeaders.h"
#include "ITCDeviceTypeEnum.h"

ITCDeviceTypeEnum MakeITCDeviceEnum(DWORD DeviceTypeInt)
{
  switch(DeviceTypeInt)
  {
  case ITC16_ID:
    return ITCDeviceTypeEnum::ITC16;
    break;
  case ITC18_ID:
    return ITCDeviceTypeEnum::ITC18;
    break;
  case ITC1600_ID:
    return ITCDeviceTypeEnum::ITC1600;
    break;
  case ITC00_ID:
    return ITCDeviceTypeEnum::ITC00;
    break;
  case USB16_ID:
    return ITCDeviceTypeEnum::ITC16USB;
    break;
  case USB18_ID:
    return ITCDeviceTypeEnum::ITC18USB;
    break;
  case ITC_ALL_DEVICES_ID:
    return ITCDeviceTypeEnum::ITCALL;
    break;
  default:
    // Unrecognised device type
    throw IgorException(INVALID_DEVICETYPE_NUMERIC,
                        "Invalid Numeric Device Type. Valid values are 0 "
                        "(ITC16), 1 (ITC18), 2 (ITC1600), 3 (ITC00), 4 "
                        "(ITC16USB), 5 (ITC18USB) and -1 (ITCALL)");
  }
}

ITCDeviceTypeEnum GetNumericDeviceType(double deviceTypeNumeric)
{

  DWORD DeviceTypeNumber = 0;
  if(deviceTypeNumeric == ITC_ALL_DEVICES_ID)
  {
    // Handle special case of -1
    // (This would cause lockToIntegerRange to raise an exception)
    DeviceTypeNumber = (DWORD) ITC_ALL_DEVICES_ID;
  }
  else
  {
    DeviceTypeNumber = lockToIntegerRange<DWORD>(deviceTypeNumeric);
  }

  return MakeITCDeviceEnum(DeviceTypeNumber);
}

ITCDeviceTypeEnum GetStringDeviceType(Handle deviceTypeString)
{
  std::string DevString = getStringFromHandle(deviceTypeString);

  if(stricmp(DevString.c_str(), "ITC16") == 0)
  {
    return ITCDeviceTypeEnum::ITC16;
  }
  else if(stricmp(DevString.c_str(), "ITC18") == 0)
  {
    return ITCDeviceTypeEnum::ITC18;
  }
  else if(stricmp(DevString.c_str(), "ITC1600") == 0)
  {
    return ITCDeviceTypeEnum::ITC1600;
  }
  else if(stricmp(DevString.c_str(), "ITC00") == 0)
  {
    return ITCDeviceTypeEnum::ITC00;
  }
  else if(stricmp(DevString.c_str(), "ITC16USB") == 0)
  {
    return ITCDeviceTypeEnum::ITC16USB;
  }
  else if(stricmp(DevString.c_str(), "ITC18USB") == 0)
  {
    return ITCDeviceTypeEnum::ITC18USB;
  }
  else if(stricmp(DevString.c_str(), "ITCALL") == 0)
  {
    return ITCDeviceTypeEnum::ITCALL;
  }
  else
  {
    throw IgorException(INVALID_DEVICETYPE_STRING,
                        "Unrecognised device type in /DTS=DeviceType "
                        "flag.\nExpected DeviceType of \"ITC16\", \"ITC18\", "
                        "\"ITC1600\", \"ITC00\", \"ITC16USB\", \"ITC18USB\", "
                        "or \"ITCALL\"");
  }
}
