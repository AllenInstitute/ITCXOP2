#include "ITCChannelTypeEnum.h"

ITCChannelTypeEnum MakeITCChannelTypeEnum(DWORD ChannelTypeInt)
{
  switch(ChannelTypeInt)
  {
  case 0:
    return ITCChannelTypeEnum::ITC_CH_IN;
  case 1:
    return ITCChannelTypeEnum::ITC_CH_OUT;
  case 2:
    return ITCChannelTypeEnum::ITC_CH_DI;
  case 3:
    return ITCChannelTypeEnum::ITC_CH_DO;
  case 4:
    return ITCChannelTypeEnum::ITC_CH_AI;
  case 5:
    return ITCChannelTypeEnum::ITC_CH_AO;
  default:
    // Unrecognised device type
    throw IgorException(INVALID_CHANNELTYPE_NUMERIC,
                        "Invalid Numeric Channel Type. Valid values are 0 (IN "
                        "/ AD), 1 (OUT / DA), 2 (DI), 3 (DO), 4 (AI) and 5 "
                        "(AO)");
  }
}

ITCChannelTypeEnum GetNumericChannelType(double channelTypeNumeric)
{
  DWORD ChannelTypeNumber = lockToIntegerRange<DWORD>(channelTypeNumeric);
  return MakeITCChannelTypeEnum(ChannelTypeNumber);
}

ITCChannelTypeEnum GetStringChannelType(Handle channelTypeString)
{
  // Read the string value
  std::string CHS = getStringFromHandle(channelTypeString);

  if(stricmp(CHS.c_str(), "IN") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_IN;
  }
  else if(stricmp(CHS.c_str(), "OUT") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_OUT;
  }
  else if(stricmp(CHS.c_str(), "AD") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_AD;
  }
  else if(stricmp(CHS.c_str(), "DA") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_DA;
  }
  else if(stricmp(CHS.c_str(), "DI") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_DI;
  }
  else if(stricmp(CHS.c_str(), "DO") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_DO;
  }
  else if(stricmp(CHS.c_str(), "AI") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_AI;
  }
  else if(stricmp(CHS.c_str(), "AO") == 0)
  {
    return ITCChannelTypeEnum::ITC_CH_AO;
  }
  else
  {
    throw IgorException(INVALID_CHANNELTYPE_STRING,
                        "Unrecognised channel type in /CHS=ChannelType "
                        "flag.\nExpected ChannelType of \"IN\", \"OUT\", "
                        "\"DA\", \"AD\", \"DI\", \"DO\", \"AI\" or \"AO\"");
  }
}
