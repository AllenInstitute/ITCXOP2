#pragma once

#include "ITC_StandardHeaders.h"
#include <Windows.h>
#include "itcmm.h"
#include "itcXOP2.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

enum class ITCChannelTypeEnum : DWORD
{
  ITC_CH_IN  = INPUT_GROUP,
  ITC_CH_OUT = OUTPUT_GROUP,
  ITC_CH_AD  = D2H,
  ITC_CH_DA  = H2D,
  ITC_CH_DI  = DIGITAL_INPUT,
  ITC_CH_DO  = DIGITAL_OUTPUT,
  ITC_CH_AI  = AUX_INPUT,
  ITC_CH_AO  = AUX_OUTPUT
};

ITCChannelTypeEnum MakeITCChannelTypeEnum(DWORD ChannelTypeInt);

/// @brief Converts a channel type number to an ITCChannelTypeEnum value
///
/// Performs bounds checking on the allowed values.
///
/// @param[in] channelTypeNumeric -- channel type number passed from Igor
///
/// @return ChannelType -- converted value
///
/// @throw Igor exception on invalid channel type
ITCChannelTypeEnum GetNumericChannelType(double channelTypeNumeric);

/// @brief Converts a channel type string to an ITCChannelTypeEnum value
///
/// Throws an exception if the channel type string does not match
/// one of the acceptable values
///
/// @param[in] channelTypeString -- channel type string passed from Igor
///
/// @return ChannelType -- converted value
///
/// @throw Igor exception on invalid channel type
ITCChannelTypeEnum GetStringChannelType(Handle channelTypeString);

/// @brief Read channel type from /CHN and /CHS operation flags
///
/// @param p -- Operation parameter struct
///
/// @return ChannelType -- converted value
///
/// @throw Igor exception on invalid flags
template <typename T>
ITCChannelTypeEnum GetChannelTypeFromParameters(T *p)
{
  if(p->CHNFlagEncountered && p->CHSFlagEncountered)
  {
    ITCChannelTypeEnum channelTypeNumeric =
        GetNumericChannelType(p->channelTypeNumeric);
    ITCChannelTypeEnum channelTypeString =
        GetStringChannelType(p->channelTypeString);

    // Cast to DWORD to account for the fact that:
    // /CHS="IN" and /CHS="AD" both map to /CHN=0
    // /CHS="OUT" and /CHS="DA" both map to /CHN=1
    // However,
    // /CHS="AD" is mapped to ITC_CH_AD, while /CHN=0 is mapped to ITC_CH_IN
    // (similarly with /CHS="DA" and /CHN=1)
    //
    // If we just did direct comparison of the enum values, then
    // this would give rise to an error, despite being OK.
    //
    // Comparing the DWORD value gets around this.
    if((DWORD) channelTypeNumeric == (DWORD) channelTypeString)
    {
      return channelTypeString;
    }
    else
    {
      throw IgorException(CHN_CHS_DISAGREE);
    }
  }

  if(p->CHNFlagEncountered)
  {
    return GetNumericChannelType(p->channelTypeNumeric);
  }

  if(p->CHSFlagEncountered)
  {
    return GetStringChannelType(p->channelTypeString);
  }

  // Neither CHN or CHS encountered.
  throw IgorException(MUST_SPECIFY_CHN_OR_CHS);
}
