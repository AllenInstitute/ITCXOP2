#include "ITC_StandardHeaders.h"

// Operation template:
// ITCReadTimer2/Z[=number:displayErrors]/DEV=number:deviceID/O/FREE
// number:mode, DataFolderAndName:{dest,real}

namespace
{

enum class ModeFlag : DWORD
{
  MODE_READ_TOTALTIME = READ_TOTALTIME,
  MODE_READ_RUNTIME   = READ_RUNTIME,
  MODE_READ_BOTH      = READ_TOTALTIME | READ_RUNTIME
};

ModeFlag ReadMode(ITCReadTimer2RuntimeParamsPtr p)
{
  if(p->modeEncountered)
  {
    DWORD modeVal = lockToIntegerRange<DWORD>(p->mode);
    switch(modeVal)
    {
    case 1:
      return ModeFlag::MODE_READ_TOTALTIME;
    case 2:
      return ModeFlag::MODE_READ_RUNTIME;
    case 3:
      return ModeFlag::MODE_READ_BOTH;
    default:
      throw IgorException(kParameterOutOfRange);
    }
  }

  // Mode not encountered
  throw IgorException(OPAND_MISMATCH);
}

} // anonymous namespace

extern "C" int ExecuteITCReadTimer2(ITCReadTimer2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  ITCStatus lStatus = {};

  // Main parameters.
  ModeFlag mode         = ReadMode(p);
  lStatus.CommandStatus = (DWORD) mode;

  // Get the value
  ITCDLL::ITC_GetState(DeviceID, &lStatus);

  // Make the destination wave
  waveHndl waveH                    = nullptr;
  const int dataType                = NT_FP64;
  std::vector<std::string> RowNames = {"TotalSeconds", "RunSeconds"};
  MakeDestWave(p, &waveH, dataType, (int) RowNames.size(), 0, RowNames, {});

  // Store the data
  double *dp = (double *) WaveData(waveH);

  switch(mode)
  {
  case ModeFlag::MODE_READ_TOTALTIME:
    dp[0] = lStatus.TotalSeconds;
    dp[1] = 0;
    break;
  case ModeFlag::MODE_READ_RUNTIME:
    dp[0] = 0;
    dp[1] = lStatus.RunSeconds;
    break;
  case ModeFlag::MODE_READ_BOTH:
    dp[0] = lStatus.TotalSeconds;
    dp[1] = lStatus.RunSeconds;
    break;
  default:
    throw IgorException(BAD_VALUE);
  }

  // Finish the destination wave.
  FinishDestWave(p, waveH);

  END_OUTER_CATCH
}
