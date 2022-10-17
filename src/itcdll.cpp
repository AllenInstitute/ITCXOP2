#include "XOPStandardHeaders.h"
#include "itcdll.h"
#include "HelperFunctions.h"
#include "CustomExceptions.h"
#include "Shlwapi.h"

#include "ConcurrentXOPNotice.h"
#include "Logging.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#define DEBUGPRINT_SIZEOF(A)                                                   \
  fmt::format_to(std::back_inserter(xd.buf),                                   \
                 FMT_STRING("sizeof(" #A ")={}\r"), sizeof(A));

namespace
{
class OutputToLogfileOnDestruct
{
public:
  OutputToLogfileOnDestruct()
  {
    if(RunningInMainThread())
    {
      fmt::format_to(std::back_inserter(buf), "(main thread)\r");
    }
    else
    {
      fmt::format_to(std::back_inserter(buf), FMT_STRING("(thread {})\r"),
                     GetCurrentThreadId());
    }
  }

  ~OutputToLogfileOnDestruct()
  {
    AddLogEntry(to_string(buf));
  }

  fmt::memory_buffer buf;
};

void DebugOut(const std::string &caller,
              const std::vector<ITCChannelInfo> &chanInfo)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCChannelInfo);

  for(size_t i = 0; i < chanInfo.size(); i++)
  {
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ModeNumberOfPoints={}\r"), i,
                   chanInfo[i].ModeNumberOfPoints);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ChannelType={}\r"), i,
                   chanInfo[i].ChannelType);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ChannelNumber={}\r"), i,
                   chanInfo[i].ChannelNumber);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ChannelMode={}\r"), i,
                   chanInfo[i].ChannelMode);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ErrorMode={}\r"), i,
                   chanInfo[i].ErrorMode);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ErrorState={}\r"), i,
                   chanInfo[i].ErrorState);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].FIFOPointer={:#x}\r"), i,
                   (UINT_PTR) chanInfo[i].FIFOPointer);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].FIFONumberOfPoints={}\r"), i,
                   chanInfo[i].FIFONumberOfPoints);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ModeOfOperation={}\r"), i,
                   chanInfo[i].ModeOfOperation);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].SizeOfModeParameters={}\r"), i,
                   chanInfo[i].SizeOfModeParameters);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ModeParameters={:#x}\r"), i,
                   (UINT_PTR) chanInfo[i].ModeParameters);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].SamplingIntervalFlag={}\r"), i,
                   chanInfo[i].SamplingIntervalFlag);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].SamplingRate={}\r"), i,
                   chanInfo[i].SamplingRate);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].StartOffset={}\r"), i,
                   chanInfo[i].StartOffset);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].Gain={}\r"), i, chanInfo[i].Gain);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].Offset={}\r"), i,
                   chanInfo[i].Offset);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].ExternalDecimation={}\r"), i,
                   chanInfo[i].ExternalDecimation);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("ChanInfo[{}].HardwareUnderrunValue={}\r"), i,
                   chanInfo[i].HardwareUnderrunValue);
  }
}

void DebugOut(const std::string &caller,
              const std::vector<ITCChannelDataEx> &chanDataEx)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCChannelDataEx);

  for(size_t i = 0; i < chanDataEx.size(); i++)
  {
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].ChannelType={}\r"), i,
                   chanDataEx[i].ChannelType);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].Command={}\r"), i,
                   chanDataEx[i].Command);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].ChannelNumber={}\r"), i,
                   chanDataEx[i].ChannelNumber);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].Status={}\r"), i,
                   chanDataEx[i].Status);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].Value={}\r"), i,
                   chanDataEx[i].Value);
    fmt::format_to(std::back_inserter(xd.buf),
                   FMT_STRING("chanDataEx[{}].DataPointer={:#x}\r"), i,
                   (UINT_PTR) chanDataEx[i].DataPointer);
  }
}

void DebugOut(const std::string &caller, const ITCPublicConfig &config)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;

  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCPublicConfig);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.DigitalInputModed=[{}]\r"),
                 config.DigitalInputMode);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ExternalTriggerModed=[{}]\r"),
                 config.ExternalTriggerMode);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ExternalTriggerd=[{}]\r"),
                 config.ExternalTrigger);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.EnableExternalClockd=[{}]\r"),
                 config.EnableExternalClock);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.DACShiftValued=[{}]\r"),
                 config.DACShiftValue);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.InputRanged=[{}]\r"), config.InputRange);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.TriggerOutPositiond=[{}]\r"),
                 config.TriggerOutPosition);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.OutputEnabled=[{}]\r"),
                 config.OutputEnable);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.SequenceLengthd=[{}]\r"),
                 config.SequenceLength);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Sequenced=[{}]\r"),
                 (UINT_PTR) config.Sequence);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.SequenceLengthInd=[{}]\r"),
                 config.SequenceLengthIn);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.SequenceInd=[{}]\r"),
                 (UINT_PTR) config.SequenceIn);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ResetFIFOFlagd=[{}]\r"),
                 config.ResetFIFOFlag);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ControlLightd=[{}]\r"),
                 config.ControlLight);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.SamplingIntervald=[{}]\r"),
                 config.SamplingInterval);
}

void DebugOut(const std::string &caller, const GlobalDeviceInfo &deviceInfo)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(GlobalDeviceInfo);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.DeviceType=[{}]\r"),
                 deviceInfo.DeviceType);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.DeviceNumber=[{}]\r"),
                 deviceInfo.DeviceNumber);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.PrimaryFIFOSize=[{}]\r"),
                 deviceInfo.PrimaryFIFOSize);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.SecondaryFIFOSize=[{}]\r"),
                 deviceInfo.SecondaryFIFOSize);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.LoadedFunction=[{}]\r"),
                 deviceInfo.LoadedFunction);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.SoftKey=[{}]\r"), deviceInfo.SoftKey);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.Mode=[{}]\r"), deviceInfo.Mode);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.MasterSerialNumber=[{}]\r"),
                 deviceInfo.MasterSerialNumber);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.SecondarySerialNumber=[{}]\r"),
                 deviceInfo.SecondarySerialNumber);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.HostSerialNumber=[{}]\r"),
                 deviceInfo.HostSerialNumber);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfDACs=[{}]\r"),
                 deviceInfo.NumberOfDACs);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfADCs=[{}]\r"),
                 deviceInfo.NumberOfADCs);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfDOs=[{}]\r"),
                 deviceInfo.NumberOfDOs);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfDIs=[{}]\r"),
                 deviceInfo.NumberOfDIs);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfAUXOs=[{}]\r"),
                 deviceInfo.NumberOfAUXOs);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.NumberOfAUXIs=[{}]\r"),
                 deviceInfo.NumberOfAUXIs);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.MinimumSamplingInterval=[{}]\r"),
                 deviceInfo.MinimumSamplingInterval);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.MinimumSamplingStep=[{}]\r"),
                 deviceInfo.MinimumSamplingStep);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.FirmwareVersion0=[{}]\r"),
                 deviceInfo.FirmwareVersion0);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("deviceInfo.Reserved1=[{}]\r"),
                 deviceInfo.Reserved1);
}

void DebugOut(const std::string &caller, const ITCStatus &status)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCStatus);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.CommandStatus=[{}]\r"),
                 status.CommandStatus);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.RunningMode=[{}]\r"), status.RunningMode);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.Overflow=[{}]\r"), status.Overflow);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.Clipping=[{}]\r"), status.Clipping);
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("status.State=[{}]\r"),
                 status.State);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.Reserved0=[{}]\r"), status.Reserved0);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.Reserved1=[{}]\r"), status.Reserved1);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.Reserved2=[{}]\r"), status.Reserved2);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.TotalSeconds=[{}]\r"), status.TotalSeconds);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("status.RunSeconds=[{}]\r"), status.RunSeconds);
}

void DebugOut(const std::string &caller, const ITCGlobalConfig &config)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCGlobalConfig);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.SoftwareFIFOSize=[{}]\r"),
                 config.SoftwareFIFOSize);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.HardwareFIFOSize_A=[{}]\r"),
                 config.HardwareFIFOSize_A);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.HardwareFIFOSize_B=[{}]\r"),
                 config.HardwareFIFOSize_B);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.TransferSizeLimitation=[{}]\r"),
                 config.TransferSizeLimitation);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Reserved0=[{}]\r"), config.Reserved0);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Reserved1=[{}]\r"), config.Reserved1);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Reserved2=[{}]\r"), config.Reserved2);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Reserved3=[{}]\r"), config.Reserved3);
}

void DebugOut(const std::string &caller, const ITCStartInfo &config)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;

  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(ITCStartInfo);

  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ExternalTrigger=[{}]\r"),
                 config.ExternalTrigger);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.OutputEnable=[{}]\r"), config.OutputEnable);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.StopOnOverflow=[{}]\r"),
                 config.StopOnOverflow);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.StopOnUnderrun=[{}]\r"),
                 config.StopOnUnderrun);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.RunningOption=[{}]\r"),
                 config.RunningOption);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.ResetFIFOs=[{}]\r"), config.ResetFIFOs);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.NumberOf640usToRun=[{}]\r"),
                 config.NumberOf640usToRun);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.Reserved3=[{}]\r"), config.Reserved3);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.StartTime=[{}]\r"), config.StartTime);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("config.StopTime=[{}]\r"), config.StopTime);
}

void DebugOut(const std::string &caller, const HWFunction &func)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}\r"), caller);
  DEBUGPRINT_SIZEOF(HWFunction);

  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("func.Mode={}\r"),
                 func.Mode);
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("func.U2F_File={}\r"),
                 func.U2F_File);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("func.SizeOfSpecialFunction={}\r"),
                 func.SizeOfSpecialFunction);
  fmt::format_to(std::back_inserter(xd.buf),
                 FMT_STRING("func.SpecialFunction={}\r"), func.SpecialFunction);
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("func.Reserved={}\r"),
                 func.Reserved);
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("func.id={}\r"),
                 func.id);
}

} // anonymous namespace

void ITCDLL::ITC_AnalyzeError(LONG Status, char *Text, DWORD MaxCharacters)
{
  DebugOut("ITC_AnalyzeError", fmt::format(FMT_STRING("Status={}"), Status));

  if(DWORD ErrorCode = ::ITC_AnalyzeError(Status, Text, MaxCharacters))
  {
    throw ITCException(ErrorCode, nullptr, "ITC_AnalyseError");
  }
}

void ITCDLL::ITC_AsyncIO(const DeviceIDHelper &DeviceID,
                         std::vector<ITCChannelDataEx> *Channels)
{
  if(DWORD ErrorCode = ::ITC_AsyncIO(
         DeviceID.getHandle(), (DWORD) Channels->size(), Channels->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_AsyncIO");
  }

  DebugOut("ITC_AsyncIO", *Channels);
}

void ITCDLL::ITC_CloseDevice(HANDLE DeviceHandle)
{
  DebugOut("ITC_CloseDevice", "called");

  if(DWORD ErrorCode = ::ITC_CloseDevice(DeviceHandle))
  {
    throw ITCException(ErrorCode, DeviceHandle, "ITC_CloseDevice");
  }
}

void ITCDLL::ITC_CloseDevice(const DeviceIDHelper &DeviceID)
{
  ITC_CloseDevice(DeviceID.getHandle());
}

void ITCDLL::ITC_ConfigDevice(HANDLE DeviceHandle, ITCPublicConfig *sITCConfig)
{
  if(DWORD ErrorCode = ::ITC_ConfigDevice(DeviceHandle, sITCConfig))
  {
    throw ITCException(ErrorCode, DeviceHandle, "ITC_ConfigDevice");
  }

  DebugOut("ITC_ConfigDevice", *sITCConfig);
}

void ITCDLL::ITC_ConfigDevice(const DeviceIDHelper &DeviceID,
                              ITCPublicConfig *sITCConfig)
{
  ITC_ConfigDevice(DeviceID.getHandle(), sITCConfig);
}

void ITCDLL::ITC_Devices(ITCDeviceTypeEnum DeviceType, DWORD *DeviceNumber)
{
  DebugOut("ITC_Devices", "called");

  if(DWORD ErrorCode = ::ITC_Devices((DWORD) DeviceType, DeviceNumber))
  {
    throw ITCException(ErrorCode, nullptr, "ITC_Devices");
  }
}

void ITCDLL::ITC_GetChannels(const DeviceIDHelper &DeviceID,
                             std::vector<ITCChannelInfo> *channels)
{
  if(DWORD ErrorCode = ::ITC_GetChannels(
         DeviceID.getHandle(), (DWORD) channels->size(), channels->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetChannels");
  }

  DebugOut("ITC_GetChannels", *channels);
}

void ITCDLL::ITC_GetDataAvailable(const DeviceIDHelper &DeviceID,
                                  std::vector<ITCChannelDataEx> *channels)
{
  if(DWORD ErrorCode = ::ITC_GetDataAvailable(
         DeviceID.getHandle(), (DWORD) channels->size(), channels->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetDataAvailable");
  }

  DebugOut("ITC_GetDataAvailable", *channels);
}

void ITCDLL::ITC_GetDeviceInfo(const DeviceIDHelper &DeviceID,
                               GlobalDeviceInfo *sDeviceInfo)
{
  if(DWORD ErrorCode = ::ITC_GetDeviceInfo(DeviceID.getHandle(), sDeviceInfo))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetDeviceInfo");
  }

  DebugOut("ITC_GetDeviceInfo", *sDeviceInfo);
}

void ITCDLL::ITC_GetDeviceType(const DeviceIDHelper &DeviceID,
                               ITCDeviceTypeEnum *DeviceType,
                               DWORD *DeviceNumber)
{
  // Set up the device type pointer.
  // If nullptr was supplied in DeviceType, use nullptr
  // Otherwise, make a new DWORD ptr
  DWORD DeviceTypeVal = (DWORD) -1;
  DWORD *DeviceTypePtr;
  if(DeviceType != nullptr)
  {
    DeviceTypeVal = (DWORD) (*DeviceType);
    DeviceTypePtr = &DeviceTypeVal;
  }
  else
  {
    DeviceTypePtr = nullptr;
  }

  if(DWORD ErrorCode =
         ::ITC_GetDeviceType(DeviceID.getHandle(), DeviceTypePtr, DeviceNumber))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetDeviceType");
  }

  // If requested, convert and return the device type
  if(DeviceType != nullptr)
  {
    *DeviceType = MakeITCDeviceEnum(DeviceTypeVal);
  }
}

void ITCDLL::ITC_GetState(const DeviceIDHelper &DeviceID, ITCStatus *lITCStatus)
{
  if(DWORD ErrorCode = ::ITC_GetState(DeviceID.getHandle(), lITCStatus))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetState");
  }

  DebugOut("ITC_GetState", *lITCStatus);
}

void ITCDLL::ITC_GetStatusText(HANDLE deviceHandle, LONG Status, char *Text,
                               DWORD MaxCharacters)
{
  DebugOut("ITC_GetStatusText", fmt::format(FMT_STRING("Status={}"), Status));

  if(DWORD ErrorCode =
         ::ITC_GetStatusText(deviceHandle, Status, Text, MaxCharacters))
  {
    throw ITCException(ErrorCode, deviceHandle, "ITC_GetStatusText");
  }
}

void ITCDLL::ITC_GetVersions(const DeviceIDHelper &DeviceID,
                             VersionInfo *ThisDriverVersion,
                             VersionInfo *KernelLevelDriverVersion,
                             VersionInfo *HardwareVersion)
{
  DebugOut("ITC_GetVersions", "called");

  if(DWORD ErrorCode =
         ::ITC_GetVersions(DeviceID.getHandle(), ThisDriverVersion,
                           KernelLevelDriverVersion, HardwareVersion))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_GetVersions");
  }
}

void ITCDLL::ITC_GlobalConfig(ITCGlobalConfig *GlobalConfig)
{
  if(DWORD ErrorCode = ::ITC_GlobalConfig(GlobalConfig))
  {
    throw ITCException(ErrorCode, nullptr, "ITC_GlobalConfig");
  }

  DebugOut("ITC_GlobalConfig", *GlobalConfig);
}

void ITCDLL::ITC_InitDevice(const DeviceIDHelper &DeviceID,
                            HWFunction *sHWFunction)
{
  if(sHWFunction != nullptr)
  {
    DebugOut("ITC_InitDevice", *sHWFunction);
  }
  else
  {
    DebugOut("ITC_InitDevice", "sHWFunction == nullptr");
  }

  if(DWORD ErrorCode = ::ITC_InitDevice(DeviceID.getHandle(), sHWFunction))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_InitDevice");
  }
}

void ITCDLL::ITC_OpenDevice(ITCDeviceTypeEnum DeviceType, DWORD DeviceNumber,
                            DWORD Mode, HANDLE *DeviceHandle)
{
  DebugOut("ITC_OpenDevice",
           fmt::format(FMT_STRING("DeviceType={}\rDeviceNumber={}\rMode={}"),
                       (DWORD) DeviceType, DeviceNumber, Mode));

  if(DWORD ErrorCode =
         ::ITC_OpenDevice((DWORD) DeviceType, DeviceNumber, Mode, DeviceHandle))
  {
    throw ITCException(ErrorCode, nullptr, "ITC_OpenDevice");
  }
}

void ITCDLL::ITC_ResetChannels(const DeviceIDHelper &DeviceID)
{
  DebugOut("ITC_ResetChannels", "called");

  if(DWORD ErrorCode = ::ITC_ResetChannels(DeviceID.getHandle()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_ResetChannels");
  }
}

void ITCDLL::ITC_SetChannels(const DeviceIDHelper &DeviceID,
                             std::vector<ITCChannelInfo> *channels)
{
  if(channels->empty())
  {
    throw IgorException(ITC_DLL_ERROR);
  }

  if(DWORD ErrorCode = ::ITC_SetChannels(
         DeviceID.getHandle(), (DWORD) channels->size(), channels->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_SetChannels");
  }

  DebugOut("ITC_SetChannels", *channels);
}

void ITCDLL::ITC_SetSoftKey(const DeviceIDHelper &DeviceID, DWORD SoftKey)
{
  DebugOut("ITC_SetSoftKey", fmt::format(FMT_STRING("SoftKey={}"), SoftKey));

  if(DWORD ErrorCode = ::ITC_SetSoftKey(DeviceID.getHandle(), SoftKey))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_SetSoftKey");
  }
}

void ITCDLL::ITC_SetState(const DeviceIDHelper &DeviceID, ITCStatus *lITCStatus)
{
  if(DWORD ErrorCode = ::ITC_SetState(DeviceID.getHandle(), lITCStatus))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_SetState");
  }

  DebugOut("ITC_SetState", *lITCStatus);
}

void ITCDLL::ITC_Start(const DeviceIDHelper &DeviceID, ITCStartInfo *sParam)
{
  if(DWORD ErrorCode = ::ITC_Start(DeviceID.getHandle(), sParam))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_Start");
  }

  if(sParam != nullptr)
  {
    DebugOut("ITC_Start", *sParam);
  }
}

void ITCDLL::ITC_Stop(const DeviceIDHelper &DeviceID, void *sParam)
{
  DebugOut("ITC_Stop", fmt::format(FMT_STRING("sParam={}"), sParam));

  if(DWORD ErrorCode = ::ITC_Stop(DeviceID.getHandle(), sParam))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_Stop");
  }
}

void ITCDLL::ITC_UpdateChannels(const DeviceIDHelper &DeviceID)
{
  DebugOut("ITC_UpdateChannels", "called");

  if(DWORD ErrorCode = ::ITC_UpdateChannels(DeviceID.getHandle()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_UpdateChannels");
  }
}

void ITCDLL::ITC_UpdateFIFOPosition(
    const DeviceIDHelper &DeviceID,
    std::vector<ITCChannelDataEx> *channelDataExVec)
{
  if(channelDataExVec->empty())
  {
    throw IgorException(ITC_DLL_ERROR);
  }

  if(DWORD ErrorCode = ::ITC_UpdateFIFOPosition(
         DeviceID.getHandle(), (DWORD) channelDataExVec->size(),
         channelDataExVec->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(),
                       "ITC_UpdateFIFOPosition");
  }

  DebugOut("ITC_UpdateFIFOPosition", *channelDataExVec);
}

void ITCDLL::ITC_ReadWriteFIFO(const DeviceIDHelper &DeviceID,
                               std::vector<ITCChannelDataEx> *channelDataExVec)
{
  if(channelDataExVec->empty())
  {
    throw IgorException(ITC_DLL_ERROR);
  }

  if(DWORD ErrorCode = ::ITC_ReadWriteFIFO(DeviceID.getHandle(),
                                           (DWORD) channelDataExVec->size(),
                                           channelDataExVec->data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_ReadWriteFIFO");
  }

  DebugOut("ITC_ReadWriteFIFO", *channelDataExVec);
}

void DebugOut(const std::string &caller, const std::string &msg)
{
  if(!debuggingEnabled)
    return;

  OutputToLogfileOnDestruct xd;
  fmt::format_to(std::back_inserter(xd.buf), FMT_STRING("Caller {}: {}\r"),
                 caller, msg);
}
