#include "ITC_StandardHeaders.h"

namespace
{

void writeSerialNumberWave(ITCGetSerialNumber2RuntimeParamsPtr p,
                           const GlobalDeviceInfo &DeviceInfo)
{

  // ResultWave: 32-bit unsigned wave with at least 3 entries :
  const int numRows = 3;
  const int numCols = 0; // Make a 1D wave

  std::vector<std::string> RowNames{
      "HostSerialNumber",      // Row 0:	Host serial number
      "MasterSerialNumber",    // Row 1:	Master serial number
      "SecondarySerialNumber", // Row 2:	Secondary serial number
  };
  std::vector<std::string> ColNames;

  // Destination wave handle
  waveHndl h = nullptr;

  const int dataType = NT_I32 | NT_UNSIGNED;
  MakeDestWave(p, &h, dataType, numRows, numCols, RowNames, ColNames);

  uint32_t *DeviceInfoWave = (uint32_t *) WaveData(h);

  // Copy the data across
  DeviceInfoWave[0] = DeviceInfo.HostSerialNumber;
  DeviceInfoWave[1] = DeviceInfo.MasterSerialNumber;
  DeviceInfoWave[2] = DeviceInfo.SecondarySerialNumber;

  FinishDestWave(p, h);
}

} // anonymous namespace

// Operation template:
// ITCGetSerialNumber2/Z[=number:displayErrors]/DEV=number:deviceID/O/FREE
// DataFolderAndName:{dest,real}

extern "C" int ExecuteITCGetSerialNumber2(ITCGetSerialNumber2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Get the channel information
  GlobalDeviceInfo DeviceInfo = {};
  ITCDLL::ITC_GetDeviceInfo(DeviceID, &DeviceInfo);

  // Copy to the output wave
  writeSerialNumberWave(p, DeviceInfo);

  END_OUTER_CATCH
}
