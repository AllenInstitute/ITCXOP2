#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCGetState2/Z[=number:displayErrors]/DEV=number:deviceID/R/OF/C/E/ALL
// /O/FREE DataFolderAndName:{dest,real}

extern "C" int ExecuteITCGetState2(ITCGetState2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  ITCStatus lStatus   = {};
  DWORD CommandStatus = 0;

  if(p->RFlagEncountered)
  {
    CommandStatus |= READ_RUNNINGMODE;
  }

  if(p->OFFlagEncountered)
  {
    CommandStatus |= READ_OVERFLOW;
  }

  if(p->CFlagEncountered)
  {
    CommandStatus |= READ_CLIPPING;
  }

  if(p->EFlagEncountered)
  {
    CommandStatus |= READ_ERRORS;
  }

  if(p->ALLFlagEncountered)
  {
    CommandStatus |= READ_RUNNINGMODE;
    CommandStatus |= READ_OVERFLOW;
    CommandStatus |= READ_CLIPPING;
    CommandStatus |= READ_ERRORS;
  }

  // Call the function
  lStatus.CommandStatus = CommandStatus;
  ITCDLL::ITC_GetState(DeviceID, &lStatus);

  // Make the destination wave
  waveHndl waveH                    = nullptr;
  const int dataType                = NT_I32 | NT_UNSIGNED;
  std::vector<std::string> RowNames = {"RunningMode", "Overflow", "Clipping",
                                       "State"};
  MakeDestWave(p, &waveH, dataType, (int) RowNames.size(), 0, RowNames, {});

  // Store the data
  uint32_t *dp = (uint32_t *) WaveData(waveH);

  dp[0] = lStatus.RunningMode;
  dp[1] = lStatus.Overflow;
  dp[2] = lStatus.Clipping;
  dp[3] = lStatus.State;

  // Finish the destination wave.
  FinishDestWave(p, waveH);

  END_OUTER_CATCH
}
