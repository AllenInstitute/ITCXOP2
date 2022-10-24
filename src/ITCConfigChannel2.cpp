#include "ITC_StandardHeaders.h"
#include "HelperFunctions.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

namespace
{

enum class sFlagOptions : DWORD
{
  s_scale  = 0,
  ms_scale = MS_SCALE,
  us_scale = US_SCALE,
  ns_scale = NS_SCALE
};

sFlagOptions ReadSFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{

  if(p->SFlagEncountered)
  {
    // /S=n
    // Parameter: p->scale
    size_t scaleParam = lockToIntegerRange<size_t>(p->scale);
    switch(scaleParam)
    {
    case 0: // s scaling
      return sFlagOptions::s_scale;
    case 1: // ms scale
      return sFlagOptions::ms_scale;
    case 2: // us scale
      return sFlagOptions::us_scale;
    case 3:
      return sFlagOptions::ns_scale;
    default:
      // Unrecognised value
      throw IgorException(
          ITCCONFIGCHANNEL2_BAD_S,
          "Bad value for /S flag.  Expected a value in the range 0..3");
    }
  }

  // Flag not encountered.
  // Default to us scale
  return sFlagOptions::us_scale;
}

enum class mFlagOptions : DWORD
{
  mode_time      = USE_TIME,
  mode_frequency = USE_FREQUENCY,
  mode_ticks     = USE_TICKS
};

mFlagOptions ReadMFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->MFlagEncountered)
  {
    // /M=n
    // Parameter: p->mode
    size_t modeParam = lockToIntegerRange<size_t>(p->mode);
    switch(modeParam)
    {
    case 0: // frequency
      return mFlagOptions::mode_frequency;
    case 1: // time
      return mFlagOptions::mode_time;
    case 2: // ticks
      return mFlagOptions::mode_ticks;
    default:
      throw IgorException(
          ITCCONFIGCHANNEL2_BAD_M,
          "Bad value for /M flag.  Expected a value in the range 0..2");
    }
  }

  // M flag not encountered
  return mFlagOptions::mode_time;
}

enum class aFlagOptions : DWORD
{
  do_not_adjust_rate = 0,
  adjust_rate        = ADJUST_RATE
};

aFlagOptions ReadAFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->AFlagEncountered)
  {
    if(p->AFlagParamsSet[0])
    {
      // A=n
      // Optional parameter: p->adjustRate
      size_t adjustRateParam = lockToIntegerRange<size_t>(p->adjustRate);
      switch(adjustRateParam)
      {
      case 0:
        return aFlagOptions::do_not_adjust_rate;
      case 1:
        return aFlagOptions::adjust_rate;
      default:
        throw IgorException(
            ITCCONFIGCHANNEL2_BAD_A,
            "Bad value for /A flag.  Expected /A, /A=0 or /A=1");
      }
    }
    // /A
    return aFlagOptions::adjust_rate;
  }

  // A flag not encountered
  return aFlagOptions::do_not_adjust_rate;
}

enum class oFlagOptions : DWORD
{
  doNotStop         = 0,
  stopChOnOverflow  = ITC_STOP_CH_ON_OVERFLOW,
  stopDrOnOverflow  = ITC_STOP_DR_ON_OVERFLOW,
  stopAllOnOverflow = ITC_STOP_ALL_ON_OVERFLOW,
  stopChOnCount     = ITC_STOP_CH_ON_COUNT,
  stopPrOnCount     = ITC_STOP_PR_ON_COUNT
};

oFlagOptions ReadOFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->OFlagEncountered)
  {
    // /O=n
    // Parameter: p->overflow
    size_t overflowParam = lockToIntegerRange<size_t>(p->overflow);

    switch(overflowParam)
    {
    case 0:
      return oFlagOptions::doNotStop;
      break;
    case 1:
      return oFlagOptions::stopChOnOverflow;
      break;
    case 2:
      return oFlagOptions::stopDrOnOverflow;
      break;
    case 3:
      return oFlagOptions::stopAllOnOverflow;
      break;
    case 4:
      return oFlagOptions::stopChOnCount;
      break;
    case 5:
      return oFlagOptions::stopPrOnCount;
      break;
    default:
      throw IgorException(
          ITCCONFIGCHANNEL2_BAD_O,
          "Bad value for /O flag.  Expected a value in the range 0..5");
    }
  }

  // O Flag not encountered.
  return oFlagOptions::doNotStop;
}

enum class uFlagOptions : DWORD
{
  doNotStop         = 0,
  stopChOnUnderrun  = ITC_STOP_CH_ON_UNDERRUN,
  stopDrOnUnderrun  = ITC_STOP_DR_ON_UNDERRUN,
  stopAllOnUnderrun = ITC_STOP_ALL_ON_UNDERRUN,
  stopChOnCount     = ITC_STOP_CH_ON_COUNT,
  stopPrOnCount     = ITC_STOP_PR_ON_COUNT
};

uFlagOptions ReadUFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->UFlagEncountered)
  {
    // /U=n
    // Parameter: p->underrun
    size_t underrunParam = lockToIntegerRange<size_t>(p->underrun);

    switch(underrunParam)
    {
    case 0:
      return uFlagOptions::doNotStop;
      break;
    case 1:
      return uFlagOptions::stopChOnUnderrun;
      break;
    case 2:
      return uFlagOptions::stopDrOnUnderrun;
      break;
    case 3:
      return uFlagOptions::stopAllOnUnderrun;
      break;
    case 4:
      return uFlagOptions::stopChOnCount;
      break;
    case 5:
      return uFlagOptions::stopPrOnCount;
      break;
    default:
      throw IgorException(
          ITCCONFIGCHANNEL2_BAD_U,
          "Bad value for /U flag.  Expected a value in the range 0..5");
    }
  }

  // U flag not encountered.
  return uFlagOptions::doNotStop;
}

DWORD ReadDFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->DFlagEncountered)
  {
    // Parameter: p->decimate
    return lockToIntegerRange<DWORD>(p->decimate);
  }

  // D flag not encountered
  return 0;
}

double ReadSamplingInterval(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->samplingIntervalEncountered)
  {
    // Parameter: p->samplingInterval
    return p->samplingInterval;
  }

  // No samplingInterval encountered
  throw IgorException(OPAND_MISMATCH);
}

DWORD ReadOFFFlagOptions(ITCConfigChannel2RuntimeParamsPtr p)
{
  if(p->OFFFlagEncountered)
  {
    // Parameter: p->decimate
    return lockToIntegerRange<DWORD>(p->offset);
  }

  // OFF flag not encountered
  return 0;
}

} // anonymous namespace

extern "C" int ExecuteITCConfigChannel2(ITCConfigChannel2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  DeviceIDHelper DeviceID(p);

  // Read the flags and parameters

  // Required flags/parameters:
  // channel type
  ITCChannelTypeEnum ChannelType = GetChannelTypeFromParameters(p);

  // Channel Number
  DWORD ChannelNumber = GetChannelNumberFromParameters<DWORD>(p);

  // Sampling Interval
  double SamplingInterval = ReadSamplingInterval(p);

  checkWave_2D(p->data, NT_I16, 1);
  waveHndl waveHandle = p->data;

  // Optional flags
  // scale flag
  sFlagOptions currSFlagOptions = ReadSFlagOptions(p);

  // mode flag
  mFlagOptions currMFlagOptions = ReadMFlagOptions(p);

  // adjust flag
  aFlagOptions currAFlagOptions = ReadAFlagOptions(p);

  // overflow flag
  oFlagOptions currOFlagOptions = ReadOFlagOptions(p);

  // underrun flag
  uFlagOptions currUFlagOptions = ReadUFlagOptions(p);

  // decimate flag
  DWORD decimate = ReadDFlagOptions(p);

  // offset flag
  DWORD offset = ReadOFFFlagOptions(p);

  DWORD NumberOfPoints = To<DWORD>(WavePoints(waveHandle));

  if(offset >= NumberOfPoints)
  {
    throw IgorException(INCOMPATIBLE_DIMENSIONING);
  }

  // Set up the options structure
  ITCChannelInfo lITCChannelInfo;
  ZeroMemory(&lITCChannelInfo, sizeof(lITCChannelInfo));

  lITCChannelInfo.SamplingIntervalFlag = (DWORD) currSFlagOptions |
                                         (DWORD) currMFlagOptions |
                                         (DWORD) currAFlagOptions;
  lITCChannelInfo.ErrorMode =
      (DWORD) currOFlagOptions | (DWORD) currUFlagOptions;
  lITCChannelInfo.ExternalDecimation = decimate;
  lITCChannelInfo.ChannelType        = (DWORD) ChannelType;
  lITCChannelInfo.ChannelNumber      = ChannelNumber;
  lITCChannelInfo.SamplingRate       = SamplingInterval;

  // TODO think about a way to let igor know when FifoPointer was updated
  // and also send the OBJ_INUSE message if igor wants to alter/delete the wave
  // FIFOPointer is pointing too
  lITCChannelInfo.FIFONumberOfPoints = NumberOfPoints - offset;
  lITCChannelInfo.FIFOPointer        = WaveData(waveHandle);

  std::vector<ITCChannelInfo> ITCChannelVec(1, lITCChannelInfo);
  ITCDLL::ITC_SetChannels(DeviceID, &ITCChannelVec);

  END_OUTER_CATCH
}
