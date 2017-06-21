#include "ITC_StandardHeaders.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

namespace
{

void writeDeviceInfoWave(ITCGetDeviceInfo2RuntimeParamsPtr p,
                         const GlobalDeviceInfo &DeviceInfo)
{

  // ResultWave: 32-bit unsigned wave with at least 20 entries :
  const int numRows = 20;
  const int numCols = 0; // Make a 1D wave

  std::vector<std::string> RowNames{

      "DeviceType",            // Row 0:	Device Type
      "DeviceNumber",          // Row 1:	Device Number
      "PrimaryFIFOSize",       // Row 2:	Primary FIFO size
      "SecondaryFIFOSize",     // Row 3:	Secondary FIFO size
      "LoadedFunction",        // Row 4:	Loaded function
      "SoftKey",               // Row 5:	SoftKey
      "Mode",                  // Row 6:	Mode
      "MasterSerialNumber",    // Row 7:	Master serial number
      "SecondarySerialNumber", // Row 8:	Secondary serial number
      "HostSerialNumber",      // Row 9:	Host serial number
      "DACCount",              // Row 10:	Number of DACs
      "ADCCount",              // Row 11:	Number of ADCs
      "DOCount",               // Row 12:	Number of digital output ports
      "DICount", // Row 13:	Number of digital input ports (each port has up to
                 // 16 lines)
      "AuxOutCount",             // Row 14:	Number of auxilary output channels
      "AuxInCount",              // Row 15:	Number of auxilary input channels
      "MinSamplingInterval",     // Row 16:	Minimum sampling interval
      "MinSamplingIntervalStep", // Row 17:	Minimum sampling interval step
      "FirmwareVersion",         // Row 18:	Firmware version
      ""                         // Row 19:	Reserved
  };
  std::vector<std::string> ColNames;

  // Destination wave handle
  waveHndl h = nullptr;

  const int dataType = NT_I32 | NT_UNSIGNED;
  MakeDestWave(p, &h, dataType, numRows, numCols, RowNames, ColNames);

  uint32_t *DeviceInfoWave = (uint32_t *) WaveData(h);

  // Copy the data across
  DeviceInfoWave[0] = DeviceInfo.DeviceType;
  DeviceInfoWave[1] = DeviceInfo.DeviceNumber;
  DeviceInfoWave[2] = DeviceInfo.PrimaryFIFOSize;
  DeviceInfoWave[3] = DeviceInfo.SecondaryFIFOSize;

  DeviceInfoWave[4] = DeviceInfo.LoadedFunction;
  DeviceInfoWave[5] = DeviceInfo.SoftKey;
  DeviceInfoWave[6] = DeviceInfo.Mode;
  DeviceInfoWave[7] = DeviceInfo.MasterSerialNumber;

  DeviceInfoWave[8]  = DeviceInfo.SecondarySerialNumber;
  DeviceInfoWave[9]  = DeviceInfo.HostSerialNumber;
  DeviceInfoWave[10] = DeviceInfo.NumberOfDACs;
  DeviceInfoWave[11] = DeviceInfo.NumberOfADCs;

  DeviceInfoWave[12] = DeviceInfo.NumberOfDOs;
  DeviceInfoWave[13] = DeviceInfo.NumberOfDIs;
  DeviceInfoWave[14] = DeviceInfo.NumberOfAUXOs;
  DeviceInfoWave[15] = DeviceInfo.NumberOfAUXIs;

  DeviceInfoWave[16] = DeviceInfo.MinimumSamplingInterval;
  DeviceInfoWave[17] = DeviceInfo.MinimumSamplingStep;
  DeviceInfoWave[18] = DeviceInfo.FirmwareVersion0;
  DeviceInfoWave[19] = DeviceInfo.Reserved1;

  FinishDestWave(p, h);
}

} // anonymous namespace

// Operation template:
// ITCGetDeviceInfo2/Z[=number:displayErrors]/DEV=number:deviceID /O /FREE
// DataFolderAndName:{dest,real}

extern "C" int ExecuteITCGetDeviceInfo2(ITCGetDeviceInfo2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Get the channel information
  GlobalDeviceInfo DeviceInfo = {};
  ITCDLL::ITC_GetDeviceInfo(DeviceID, &DeviceInfo);

  // Copy to the output wave
  writeDeviceInfoWave(p, DeviceInfo);

  END_OUTER_CATCH
}
