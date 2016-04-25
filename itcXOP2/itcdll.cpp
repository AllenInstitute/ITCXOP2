#include "XOPStandardHeaders.h"
#include "itcdll.h"
#include "HelperFunctions.h"
#include <sstream>
#include "CustomExceptions.h"
#include "Shlwapi.h"

/*
Utility functions which are xop specific
*/

// Accepts multiple arguments like printf and prints them to the history
// Custom prefixes can be also inserted, see DEBUGPRINT
// Copies only ARRAY_SIZE-2 characters in _snprintf, because we want to have
// space for the terminating \0 (1) and for the CR (1)
// Checking the return value of _snprintf is not done on purpose, we just always
// append a \0 to be safe
// @param A prints only if A evaluates to true
// @param B uses silent printing (does not mark the experiment as changed) if
// true
#define PRINT_TO_HISTORY(A, B, ...)                                            \
  if(RunningInMainThread() && A)                                               \
  {                                                                            \
    constexpr size_t ARRAY_SIZE = 1024;                                        \
    char buf[ARRAY_SIZE];                                                      \
    _snprintf(buf, ARRAY_SIZE - 2, __VA_ARGS__);                               \
    buf[ARRAY_SIZE - 2] = '\0';                                                \
    strcat(buf, CR_STR);                                                       \
    if(!B)                                                                     \
    {                                                                          \
      XOPNotice(buf);                                                          \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      XOPNotice2(buf, 0);                                                      \
    }                                                                          \
  }

// Convenience wrapper using silent debug print
#define DEBUGPRINT_SILENT(...)                                                 \
  PRINT_TO_HISTORY(debuggingEnabled, true, "DEBUG: "__VA_ARGS__)

// Convenience wrapper using debug print
#define DEBUGPRINT(...)                                                        \
  PRINT_TO_HISTORY(debuggingEnabled, false, "DEBUG: "__VA_ARGS__)

// Convenience wrapper for printing
#define HISTPRINT(...) PRINT_TO_HISTORY(true, false, __VA_ARGS__)

// Convenience wrapper for silent printing
#define HISTPRINT_SILENT(...) PRINT_TO_HISTORY(true, true, __VA_ARGS__)

#define DEBUGPRINT_SIZEOF(A) DEBUGPRINT("sizeof(" #A ")=%zd", sizeof(A));

namespace
{

void DebugOut(std::string caller, std::vector<ITCChannelInfo> chanInfo)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCChannelInfo);

  for(size_t i = 0; i < chanInfo.size(); i++)
  {
    DEBUGPRINT("ChanInfo[%zd].ModeNumberOfPoints=%d", i,
               chanInfo[i].ModeNumberOfPoints);
    DEBUGPRINT("ChanInfo[%zd].ChannelType=%d", i, chanInfo[i].ChannelType);
    DEBUGPRINT("ChanInfo[%zd].ChannelNumber=%d", i, chanInfo[i].ChannelNumber);
    DEBUGPRINT("ChanInfo[%zd].ChannelMode=%d", i, chanInfo[i].ChannelMode);
    DEBUGPRINT("ChanInfo[%zd].ErrorMode=%d", i, chanInfo[i].ErrorMode);
    DEBUGPRINT("ChanInfo[%zd].ErrorState=%d", i, chanInfo[i].ErrorState);
    DEBUGPRINT("ChanInfo[%zd].FIFOPointer=%Id", i,
               (UINT_PTR) chanInfo[i].FIFOPointer);
    DEBUGPRINT("ChanInfo[%zd].FIFONumberOfPoints=%d", i,
               chanInfo[i].FIFONumberOfPoints);
    DEBUGPRINT("ChanInfo[%zd].ModeOfOperation=%d", i,
               chanInfo[i].ModeOfOperation);
    DEBUGPRINT("ChanInfo[%zd].SizeOfModeParameters=%d", i,
               chanInfo[i].SizeOfModeParameters);
    DEBUGPRINT("ChanInfo[%zd].ModeParameters=%Id", i,
               (UINT_PTR) chanInfo[i].ModeParameters);
    DEBUGPRINT("ChanInfo[%zd].SamplingIntervalFlag=%d", i,
               chanInfo[i].SamplingIntervalFlag);
    DEBUGPRINT("ChanInfo[%zd].SamplingRate=%g", i, chanInfo[i].SamplingRate);
    DEBUGPRINT("ChanInfo[%zd].StartOffset=%g", i, chanInfo[i].StartOffset);
    DEBUGPRINT("ChanInfo[%zd].Gain=%g", i, chanInfo[i].Gain);
    DEBUGPRINT("ChanInfo[%zd].Offset=%g", i, chanInfo[i].Offset);
    DEBUGPRINT("ChanInfo[%zd].ExternalDecimation=%d", i,
               chanInfo[i].ExternalDecimation);
    DEBUGPRINT("ChanInfo[%zd].HardwareUnderrunValue=%d", i,
               chanInfo[i].HardwareUnderrunValue);
  }
}

void DebugOut(std::string caller, std::vector<ITCChannelDataEx> chanDataEx)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCChannelDataEx);

  for(size_t i = 0; i < chanDataEx.size(); i++)
  {
    DEBUGPRINT("chanDataEx[%zd].ChannelType=%d", i, chanDataEx[i].ChannelType);
    DEBUGPRINT("chanDataEx[%zd].Command=%d", i, chanDataEx[i].Command);
    DEBUGPRINT("chanDataEx[%zd].ChannelNumber=%d", i,
               chanDataEx[i].ChannelNumber);
    DEBUGPRINT("chanDataEx[%zd].Status=%d", i, chanDataEx[i].Status);
    DEBUGPRINT("chanDataEx[%zd].Value=%d", i, chanDataEx[i].Value);
    DEBUGPRINT("chanDataEx[%zd].DataPointer=%Id", i,
               (UINT_PTR) chanDataEx[i].DataPointer);
  }
}

void DebugOut(std::string caller, ITCPublicConfig config)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCPublicConfig);

  DEBUGPRINT("config.DigitalInputModed=[%d]", config.DigitalInputMode);
  DEBUGPRINT("config.ExternalTriggerModed=[%d]", config.ExternalTriggerMode);
  DEBUGPRINT("config.ExternalTriggerd=[%d]", config.ExternalTrigger);
  DEBUGPRINT("config.EnableExternalClockd=[%d]", config.EnableExternalClock);
  DEBUGPRINT("config.DACShiftValued=[%d]", config.DACShiftValue);
  DEBUGPRINT("config.InputRanged=[%d]", config.InputRange);
  DEBUGPRINT("config.TriggerOutPositiond=[%d]", config.TriggerOutPosition);
  DEBUGPRINT("config.OutputEnabled=[%d]", config.OutputEnable);
  DEBUGPRINT("config.SequenceLengthd=[%d]", config.SequenceLength);
  DEBUGPRINT("config.Sequenced=[%zd]", (UINT_PTR) config.Sequence);
  DEBUGPRINT("config.SequenceLengthInd=[%d]", config.SequenceLengthIn);
  DEBUGPRINT("config.SequenceInd=[%zd]", (UINT_PTR) config.SequenceIn);
  DEBUGPRINT("config.ResetFIFOFlagd=[%d]", config.ResetFIFOFlag);
  DEBUGPRINT("config.ControlLightd=[%d]", config.ControlLight);
  DEBUGPRINT("config.SamplingIntervald=[%g]", config.SamplingInterval);
}

void DebugOut(std::string caller, GlobalDeviceInfo deviceInfo)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(GlobalDeviceInfo);

  DEBUGPRINT("deviceInfo.DeviceType=[%d]", deviceInfo.DeviceType)
  DEBUGPRINT("deviceInfo.DeviceNumber=[%d]", deviceInfo.DeviceNumber)
  DEBUGPRINT("deviceInfo.PrimaryFIFOSize=[%d]", deviceInfo.PrimaryFIFOSize)
  DEBUGPRINT("deviceInfo.SecondaryFIFOSize=[%d]", deviceInfo.SecondaryFIFOSize)

  DEBUGPRINT("deviceInfo.LoadedFunction=[%d]", deviceInfo.LoadedFunction)
  DEBUGPRINT("deviceInfo.SoftKey=[%d]", deviceInfo.SoftKey)
  DEBUGPRINT("deviceInfo.Mode=[%d]", deviceInfo.Mode)
  DEBUGPRINT("deviceInfo.MasterSerialNumber=[%d]",
             deviceInfo.MasterSerialNumber)

  DEBUGPRINT("deviceInfo.SecondarySerialNumber=[%d]",
             deviceInfo.SecondarySerialNumber)
  DEBUGPRINT("deviceInfo.HostSerialNumber=[%d]", deviceInfo.HostSerialNumber)
  DEBUGPRINT("deviceInfo.NumberOfDACs=[%d]", deviceInfo.NumberOfDACs)
  DEBUGPRINT("deviceInfo.NumberOfADCs=[%d]", deviceInfo.NumberOfADCs)

  DEBUGPRINT("deviceInfo.NumberOfDOs=[%d]", deviceInfo.NumberOfDOs)
  DEBUGPRINT("deviceInfo.NumberOfDIs=[%d]", deviceInfo.NumberOfDIs)
  DEBUGPRINT("deviceInfo.NumberOfAUXOs=[%d]", deviceInfo.NumberOfAUXOs)
  DEBUGPRINT("deviceInfo.NumberOfAUXIs=[%d]", deviceInfo.NumberOfAUXIs)

  DEBUGPRINT("deviceInfo.MinimumSamplingInterval=[%d]",
             deviceInfo.MinimumSamplingInterval)
  DEBUGPRINT("deviceInfo.MinimumSamplingStep=[%d]",
             deviceInfo.MinimumSamplingStep)
  DEBUGPRINT("deviceInfo.FirmwareVersion0=[%d]", deviceInfo.FirmwareVersion0)
  DEBUGPRINT("deviceInfo.Reserved1=[%d]", deviceInfo.Reserved1)
}

void DebugOut(std::string caller, ITCStatus status)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCStatus);

  DEBUGPRINT("status.CommandStatus=[%d]", status.CommandStatus);
  DEBUGPRINT("status.RunningMode=[%d]", status.RunningMode);
  DEBUGPRINT("status.Overflow=[%d]", status.Overflow);
  DEBUGPRINT("status.Clipping=[%d]", status.Clipping);
  DEBUGPRINT("status.State=[%d]", status.State);
  DEBUGPRINT("status.Reserved0=[%d]", status.Reserved0);
  DEBUGPRINT("status.Reserved1=[%d]", status.Reserved1);
  DEBUGPRINT("status.Reserved2=[%d]", status.Reserved2);
  DEBUGPRINT("status.TotalSeconds=[%g]", status.TotalSeconds);
  DEBUGPRINT("status.RunSeconds=[%g]", status.RunSeconds);
}

void DebugOut(std::string caller, ITCGlobalConfig config)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCGlobalConfig);

  DEBUGPRINT("config.SoftwareFIFOSize=[%d]", config.SoftwareFIFOSize);
  DEBUGPRINT("config.HardwareFIFOSize_A=[%d]", config.HardwareFIFOSize_A);
  DEBUGPRINT("config.HardwareFIFOSize_B=[%d]", config.HardwareFIFOSize_B);
  DEBUGPRINT("config.TransferSizeLimitation=[%d]",
             config.TransferSizeLimitation);
  DEBUGPRINT("config.Reserved0=[%d]", config.Reserved0);
  DEBUGPRINT("config.Reserved1=[%d]", config.Reserved1);
  DEBUGPRINT("config.Reserved2=[%d]", config.Reserved2);
  DEBUGPRINT("config.Reserved3=[%d]", config.Reserved3);
}

void DebugOut(std::string caller, ITCStartInfo config)
{
  if(!debuggingEnabled)
    return;

  DEBUGPRINT("Caller %s", caller.c_str());
  DEBUGPRINT_SIZEOF(ITCStartInfo);

  DEBUGPRINT("config.ExternalTrigger=[%d]", config.ExternalTrigger);
  DEBUGPRINT("config.OutputEnable=[%d]", config.OutputEnable);
  DEBUGPRINT("config.StopOnOverflow=[%d]", config.StopOnOverflow);
  DEBUGPRINT("config.StopOnUnderrun=[%d]", config.StopOnUnderrun);
  DEBUGPRINT("config.RunningOption=[%d]", config.RunningOption);
  DEBUGPRINT("config.ResetFIFOs=[%d]", config.ResetFIFOs);
  DEBUGPRINT("config.NumberOf640usToRun=[%d]", config.NumberOf640usToRun);
  DEBUGPRINT("config.Reserved3=[%d]", config.Reserved3);
  DEBUGPRINT("config.StartTime=[%g]", config.StartTime);
  DEBUGPRINT("config.StopTime=[%g]", config.StopTime);
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
