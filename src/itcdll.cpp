#include "XOPStandardHeaders.h"
#include "itcdll.h"
#include "HelperFunctions.h"
#include <sstream>
#include "CustomExceptions.h"
#include "Shlwapi.h"

#include "fmt/format.h"
#include "fmt/ostream.h"
#include "fmt/printf.h"
#include "ConcurrentXOPNotice.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#define DEBUGPRINT_SIZEOF(A) fprintf(xd.sstr, "sizeof(" #A ")=%d\r", sizeof(A));

namespace
{
class XOPNoticeOnDestruct
{
public:
  XOPNoticeOnDestruct()
  {
    if(RunningInMainThread())
    {
      fmt::fprintf(sstr, "(main thread)\r");
    }
    else
    {
      fmt::fprintf(sstr, "(thread %d)\r", GetCurrentThreadId());
    }
  }

  ~XOPNoticeOnDestruct()
  {
    if(debuggingEnabled)
    {
      XOPNotice_ts(sstr.str());
    }
  }

  std::stringstream sstr;
};

void DebugOut(std::string caller, std::vector<ITCChannelInfo> chanInfo)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;
  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCChannelInfo);

  for(size_t i = 0; i < chanInfo.size(); i++)
  {
    fprintf(xd.sstr, "ChanInfo[%d].ModeNumberOfPoints=%d\r", i,
            chanInfo[i].ModeNumberOfPoints);
    fprintf(xd.sstr, "ChanInfo[%d].ChannelType=%d\r", i,
            chanInfo[i].ChannelType);
    fprintf(xd.sstr, "ChanInfo[%d].ChannelNumber=%d\r", i,
            chanInfo[i].ChannelNumber);
    fprintf(xd.sstr, "ChanInfo[%d].ChannelMode=%d\r", i,
            chanInfo[i].ChannelMode);
    fprintf(xd.sstr, "ChanInfo[%d].ErrorMode=%d\r", i, chanInfo[i].ErrorMode);
    fprintf(xd.sstr, "ChanInfo[%d].ErrorState=%d\r", i, chanInfo[i].ErrorState);
    fprintf(xd.sstr, "ChanInfo[%d].FIFOPointer=%#x\r", i,
            (UINT_PTR) chanInfo[i].FIFOPointer);
    fprintf(xd.sstr, "ChanInfo[%d].FIFONumberOfPoints=%d\r", i,
            chanInfo[i].FIFONumberOfPoints);
    fprintf(xd.sstr, "ChanInfo[%d].ModeOfOperation=%d\r", i,
            chanInfo[i].ModeOfOperation);
    fprintf(xd.sstr, "ChanInfo[%d].SizeOfModeParameters=%d\r", i,
            chanInfo[i].SizeOfModeParameters);
    fprintf(xd.sstr, "ChanInfo[%d].ModeParameters=%#x\r", i,
            (UINT_PTR) chanInfo[i].ModeParameters);
    fprintf(xd.sstr, "ChanInfo[%d].SamplingIntervalFlag=%d\r", i,
            chanInfo[i].SamplingIntervalFlag);
    fprintf(xd.sstr, "ChanInfo[%d].SamplingRate=%g\r", i,
            chanInfo[i].SamplingRate);
    fprintf(xd.sstr, "ChanInfo[%d].StartOffset=%g\r", i,
            chanInfo[i].StartOffset);
    fprintf(xd.sstr, "ChanInfo[%d].Gain=%g\r", i, chanInfo[i].Gain);
    fprintf(xd.sstr, "ChanInfo[%d].Offset=%g\r", i, chanInfo[i].Offset);
    fprintf(xd.sstr, "ChanInfo[%d].ExternalDecimation=%d\r", i,
            chanInfo[i].ExternalDecimation);
    fprintf(xd.sstr, "ChanInfo[%d].HardwareUnderrunValue=%d\r", i,
            chanInfo[i].HardwareUnderrunValue);
  }
}

void DebugOut(std::string caller, std::vector<ITCChannelDataEx> chanDataEx)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;
  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCChannelDataEx);

  for(size_t i = 0; i < chanDataEx.size(); i++)
  {
    fprintf(xd.sstr, "chanDataEx[%d].ChannelType=%d\r", i,
            chanDataEx[i].ChannelType);
    fprintf(xd.sstr, "chanDataEx[%d].Command=%d\r", i, chanDataEx[i].Command);
    fprintf(xd.sstr, "chanDataEx[%d].ChannelNumber=%d\r", i,
            chanDataEx[i].ChannelNumber);
    fprintf(xd.sstr, "chanDataEx[%d].Status=%d\r", i, chanDataEx[i].Status);
    fprintf(xd.sstr, "chanDataEx[%d].Value=%d\r", i, chanDataEx[i].Value);
    fprintf(xd.sstr, "chanDataEx[%d].DataPointer=%#x\r", i,
            (UINT_PTR) chanDataEx[i].DataPointer);
  }
}

void DebugOut(std::string caller, ITCPublicConfig config)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;

  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCPublicConfig);

  fprintf(xd.sstr, "config.DigitalInputModed=[%d]\r", config.DigitalInputMode);
  fprintf(xd.sstr, "config.ExternalTriggerModed=[%d]\r",
          config.ExternalTriggerMode);
  fprintf(xd.sstr, "config.ExternalTriggerd=[%d]\r", config.ExternalTrigger);
  fprintf(xd.sstr, "config.EnableExternalClockd=[%d]\r",
          config.EnableExternalClock);
  fprintf(xd.sstr, "config.DACShiftValued=[%d]\r", config.DACShiftValue);
  fprintf(xd.sstr, "config.InputRanged=[%d]\r", config.InputRange);
  fprintf(xd.sstr, "config.TriggerOutPositiond=[%d]\r",
          config.TriggerOutPosition);
  fprintf(xd.sstr, "config.OutputEnabled=[%d]\r", config.OutputEnable);
  fprintf(xd.sstr, "config.SequenceLengthd=[%d]\r", config.SequenceLength);
  fprintf(xd.sstr, "config.Sequenced=[%d]\r", (UINT_PTR) config.Sequence);
  fprintf(xd.sstr, "config.SequenceLengthInd=[%d]\r", config.SequenceLengthIn);
  fprintf(xd.sstr, "config.SequenceInd=[%d]\r", (UINT_PTR) config.SequenceIn);
  fprintf(xd.sstr, "config.ResetFIFOFlagd=[%d]\r", config.ResetFIFOFlag);
  fprintf(xd.sstr, "config.ControlLightd=[%d]\r", config.ControlLight);
  fprintf(xd.sstr, "config.SamplingIntervald=[%g]\r", config.SamplingInterval);
}

void DebugOut(std::string caller, GlobalDeviceInfo deviceInfo)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;
  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(GlobalDeviceInfo);

  fprintf(xd.sstr, "deviceInfo.DeviceType=[%d]\r", deviceInfo.DeviceType);
  fprintf(xd.sstr, "deviceInfo.DeviceNumber=[%d]\r", deviceInfo.DeviceNumber);
  fprintf(xd.sstr, "deviceInfo.PrimaryFIFOSize=[%d]\r",
          deviceInfo.PrimaryFIFOSize);
  fprintf(xd.sstr, "deviceInfo.SecondaryFIFOSize=[%d]\r",
          deviceInfo.SecondaryFIFOSize);

  fprintf(xd.sstr, "deviceInfo.LoadedFunction=[%d]\r",
          deviceInfo.LoadedFunction);
  fprintf(xd.sstr, "deviceInfo.SoftKey=[%d]\r", deviceInfo.SoftKey);
  fprintf(xd.sstr, "deviceInfo.Mode=[%d]\r", deviceInfo.Mode);
  fprintf(xd.sstr, "deviceInfo.MasterSerialNumber=[%d]\r",
          deviceInfo.MasterSerialNumber);

  fprintf(xd.sstr, "deviceInfo.SecondarySerialNumber=[%d]\r",
          deviceInfo.SecondarySerialNumber);
  fprintf(xd.sstr, "deviceInfo.HostSerialNumber=[%d]\r",
          deviceInfo.HostSerialNumber);
  fprintf(xd.sstr, "deviceInfo.NumberOfDACs=[%d]\r", deviceInfo.NumberOfDACs);
  fprintf(xd.sstr, "deviceInfo.NumberOfADCs=[%d]\r", deviceInfo.NumberOfADCs);

  fprintf(xd.sstr, "deviceInfo.NumberOfDOs=[%d]\r", deviceInfo.NumberOfDOs);
  fprintf(xd.sstr, "deviceInfo.NumberOfDIs=[%d]\r", deviceInfo.NumberOfDIs);
  fprintf(xd.sstr, "deviceInfo.NumberOfAUXOs=[%d]\r", deviceInfo.NumberOfAUXOs);
  fprintf(xd.sstr, "deviceInfo.NumberOfAUXIs=[%d]\r", deviceInfo.NumberOfAUXIs);

  fprintf(xd.sstr, "deviceInfo.MinimumSamplingInterval=[%d]\r",
          deviceInfo.MinimumSamplingInterval);
  fprintf(xd.sstr, "deviceInfo.MinimumSamplingStep=[%d]\r",
          deviceInfo.MinimumSamplingStep);
  fprintf(xd.sstr, "deviceInfo.FirmwareVersion0=[%d]\r",
          deviceInfo.FirmwareVersion0);
  fprintf(xd.sstr, "deviceInfo.Reserved1=[%d]\r", deviceInfo.Reserved1);
}

void DebugOut(std::string caller, ITCStatus status)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;
  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCStatus);

  fprintf(xd.sstr, "status.CommandStatus=[%d]\r", status.CommandStatus);
  fprintf(xd.sstr, "status.RunningMode=[%d]\r", status.RunningMode);
  fprintf(xd.sstr, "status.Overflow=[%d]\r", status.Overflow);
  fprintf(xd.sstr, "status.Clipping=[%d]\r", status.Clipping);
  fprintf(xd.sstr, "status.State=[%d]\r", status.State);
  fprintf(xd.sstr, "status.Reserved0=[%d]\r", status.Reserved0);
  fprintf(xd.sstr, "status.Reserved1=[%d]\r", status.Reserved1);
  fprintf(xd.sstr, "status.Reserved2=[%d]\r", status.Reserved2);
  fprintf(xd.sstr, "status.TotalSeconds=[%g]\r", status.TotalSeconds);
  fprintf(xd.sstr, "status.RunSeconds=[%g]\r", status.RunSeconds);
}

void DebugOut(std::string caller, ITCGlobalConfig config)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;
  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCGlobalConfig);

  fprintf(xd.sstr, "config.SoftwareFIFOSize=[%d]\r", config.SoftwareFIFOSize);
  fprintf(xd.sstr, "config.HardwareFIFOSize_A=[%d]\r",
          config.HardwareFIFOSize_A);
  fprintf(xd.sstr, "config.HardwareFIFOSize_B=[%d]\r",
          config.HardwareFIFOSize_B);
  fprintf(xd.sstr, "config.TransferSizeLimitation=[%d]\r",
          config.TransferSizeLimitation);
  fprintf(xd.sstr, "config.Reserved0=[%d]\r", config.Reserved0);
  fprintf(xd.sstr, "config.Reserved1=[%d]\r", config.Reserved1);
  fprintf(xd.sstr, "config.Reserved2=[%d]\r", config.Reserved2);
  fprintf(xd.sstr, "config.Reserved3=[%d]\r", config.Reserved3);
}

void DebugOut(std::string caller, ITCStartInfo config)
{
  using namespace fmt;

  if(!debuggingEnabled)
    return;

  XOPNoticeOnDestruct xd;

  fprintf(xd.sstr, "Caller %s\r", caller);
  DEBUGPRINT_SIZEOF(ITCStartInfo);

  fprintf(xd.sstr, "config.ExternalTrigger=[%d]\r", config.ExternalTrigger);
  fprintf(xd.sstr, "config.OutputEnable=[%d]\r", config.OutputEnable);
  fprintf(xd.sstr, "config.StopOnOverflow=[%d]\r", config.StopOnOverflow);
  fprintf(xd.sstr, "config.StopOnUnderrun=[%d]\r", config.StopOnUnderrun);
  fprintf(xd.sstr, "config.RunningOption=[%d]\r", config.RunningOption);
  fprintf(xd.sstr, "config.ResetFIFOs=[%d]\r", config.ResetFIFOs);
  fprintf(xd.sstr, "config.NumberOf640usToRun=[%d]\r",
          config.NumberOf640usToRun);
  fprintf(xd.sstr, "config.Reserved3=[%d]\r", config.Reserved3);
  fprintf(xd.sstr, "config.StartTime=[%g]\r", config.StartTime);
  fprintf(xd.sstr, "config.StopTime=[%g]\r", config.StopTime);
}

} // anonymous namespace

void ITCDLL::ITC_AnalyzeError(LONG Status, char *Text, DWORD MaxCharacters)
{
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
  DWORD DeviceTypeVal;
  DWORD *DeviceTypePtr;
  if(DeviceType != nullptr)
  {
    DeviceTypeVal = (DWORD)(*DeviceType);
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
    throw ITCException(ErrorCode, nullptr, "ITC_GetState");
  }

  DebugOut("ITC_GetState", *lITCStatus);
}

void ITCDLL::ITC_GetStatusText(HANDLE deviceHandle, LONG Status, char *Text,
                               DWORD MaxCharacters)
{
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
  if(DWORD ErrorCode = ::ITC_InitDevice(DeviceID.getHandle(), sHWFunction))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_InitDevice");
  }
}

void ITCDLL::ITC_OpenDevice(ITCDeviceTypeEnum DeviceType, DWORD DeviceNumber,
                            DWORD Mode, HANDLE *DeviceHandle)
{
  if(DWORD ErrorCode =
         ::ITC_OpenDevice((DWORD) DeviceType, DeviceNumber, Mode, DeviceHandle))
  {
    throw ITCException(ErrorCode, nullptr, "ITC_OpenDevice");
  }
}

void ITCDLL::ITC_ResetChannels(const DeviceIDHelper &DeviceID)
{
  if(DWORD ErrorCode = ::ITC_ResetChannels(DeviceID.getHandle()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_ResetChannels");
  }
}

void ITCDLL::ITC_SetChannels(const DeviceIDHelper &DeviceID,
                             std::vector<ITCChannelInfo> channels)
{
  if(channels.empty())
  {
    throw IgorException();
  }

  if(DWORD ErrorCode = ::ITC_SetChannels(
         DeviceID.getHandle(), (DWORD) channels.size(), channels.data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_SetChannels");
  }

  DebugOut("ITC_SetChannels", channels);
}

void ITCDLL::ITC_SetSoftKey(const DeviceIDHelper &DeviceID, DWORD SoftKey)
{
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
  if(DWORD ErrorCode = ::ITC_Stop(DeviceID.getHandle(), sParam))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_Stop");
  }
}

void ITCDLL::ITC_UpdateChannels(const DeviceIDHelper &DeviceID)
{
  if(DWORD ErrorCode = ::ITC_UpdateChannels(DeviceID.getHandle()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(), "ITC_UpdateChannels");
  }
}

void ITCDLL::ITC_UpdateFIFOPosition(
    const DeviceIDHelper &DeviceID,
    std::vector<ITCChannelDataEx> channelDataExVec)
{
  if(channelDataExVec.empty())
  {
    throw IgorException();
  }

  if(DWORD ErrorCode = ::ITC_UpdateFIFOPosition(DeviceID.getHandle(),
                                                (DWORD) channelDataExVec.size(),
                                                channelDataExVec.data()))
  {
    throw ITCException(ErrorCode, DeviceID.getHandle(),
                       "ITC_UpdateFIFOPosition");
  }

  DebugOut("ITC_UpdateFIFOPosition", channelDataExVec);
}
