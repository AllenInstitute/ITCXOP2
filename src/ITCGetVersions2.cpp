#include "ITC_StandardHeaders.h"
#include "fmt/format.h"
#include <numeric>

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Operation template:
// ITCGetVersions2/Z[=number:displayErrors]/DEV=number:deviceID /O /FREE
// DataFolderAndName:{dest,text}

namespace
{

void writeVersionWave(ITCGetVersions2RuntimeParamsPtr p,
                      const VersionInfo lVersion[3])
{
  // ResultWave: text wave with 3 rows and 4 columns :
  const int numRows = 4;
  const int numCols = 4;

  std::vector<std::string> RowNames{"Driver Version", "Kernel Version",
                                    "Hardware Version", "XOP"};
  std::vector<std::string> ColNames = {"Major", "Minor", "Description", "Date"};

  // Destination wave handle
  waveHndl waveH = nullptr;

  const int dataType = TEXT_WAVE_TYPE;
  MakeDestWave(p, &waveH, dataType, numRows, numCols, RowNames, ColNames);

  IndexInt indices[MAX_DIMENSIONS] = {};

  Handle textH = nullptr;
  textH        = NewHandle(0L);
  if(textH == NULL)
  {
    throw IgorException(NOMEM);
  }

  int RetVal = 0;
  for(IndexInt currRow = 0; currRow < 3; currRow++)
  {
    auto minor = lVersion[currRow].Minor;
    auto major = lVersion[currRow].Major;

    if(minor == 0 && major == 0)
    {
      continue;
    }

    indices[ROWS] = currRow;

    indices[COLUMNS] = 0;
    PutCStringInHandle(std::to_string(major).c_str(), textH);
    RetVal = MDSetTextWavePointValue(waveH, indices, textH);
    if(RetVal != 0)
    {
      break;
    }

    indices[COLUMNS] = 1;
    PutCStringInHandle(std::to_string(minor).c_str(), textH);
    RetVal = MDSetTextWavePointValue(waveH, indices, textH);
    if(RetVal != 0)
    {
      break;
    }

    indices[COLUMNS] = 2;
    PutCStringInHandle(lVersion[currRow].description, textH);
    RetVal = MDSetTextWavePointValue(waveH, indices, textH);
    if(RetVal != 0)
    {
      break;
    }

    indices[COLUMNS] = 3;
    PutCStringInHandle(lVersion[currRow].date, textH);
    RetVal = MDSetTextWavePointValue(waveH, indices, textH);
    if(RetVal != 0)
    {
      break;
    }
  }

  auto versionInfo = GetVersionInfo("ITCXOP2");

  auto desc = fmt::format(FMT_STRING("{}: {}"), versionInfo["name"],
                          versionInfo["version"]);

  indices[ROWS]    = 3;
  indices[COLUMNS] = 2;
  PutCStringInHandle(desc.c_str(), textH);
  RetVal = MDSetTextWavePointValue(waveH, indices, textH);

  indices[ROWS]    = 3;
  indices[COLUMNS] = 3;
  PutCStringInHandle(versionInfo["builddate"].c_str(), textH);
  RetVal = MDSetTextWavePointValue(waveH, indices, textH);

  // Always clean up.
  DisposeHandle(textH);

  // If we didn't set the text wave data properly, error.
  if(RetVal)
  {
    throw IgorException(RetVal);
  }

  FinishDestWave(p, waveH);
}

} // anonymous namespace

extern "C" int ExecuteITCGetVersions2(ITCGetVersions2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  VersionInfo lVersion[3] = {};

  // we want to output something even without a device connected
  try
  {
    // Get the deviceID and handle to use.
    DeviceIDHelper DeviceID(p);

    // Get the version information
    ITCDLL::ITC_GetVersions(DeviceID, &lVersion[0], &lVersion[1], &lVersion[2]);
  }
  catch(const IgorException &e)
  {
    // ignore non connected ITC device
    if(e.ErrorCode != SLOT_EMPTY)
    {
      throw;
    }
  }

  // Copy to the output wave
  writeVersionWave(p, lVersion);

  END_OUTER_CATCH
}
