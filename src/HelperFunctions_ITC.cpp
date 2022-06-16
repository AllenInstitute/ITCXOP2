#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

extern DeviceIDClass DeviceIDs;

namespace
{

void RemoveDevice(const DeviceIDHelper &DeviceID)
{
  // If we've just closed the device corresponding to the thread Device ID, then
  // set the device ID to invalid
  if(DeviceIDHelper::getThreadDeviceID() == DeviceID.getDeviceID())
  {
    DeviceIDHelper::setCurrentThreadDeviceID(
        DeviceIDClass::INVALID_DEVICE_INDEX);
  }

  // Remove the device ID
  DeviceIDs.remove(DeviceID.getDeviceID());
}
} // namespace

void CloseDeviceLowLevel(HANDLE DeviceHandle)
{
  try
  {
    // Turn off LED
    ITCPublicConfig lITCPublicConfig;
    ZeroMemory(&lITCPublicConfig, sizeof(lITCPublicConfig));
    ITCDLL::ITC_ConfigDevice(DeviceHandle, &lITCPublicConfig);
  }
  catch(const ITCException &e)
  {
    DebugOut("CloseDeviceLowLevel", fmt::format("Ignoring exception: {}", e));
  }

  // Close the device handle
  ITCDLL::ITC_CloseDevice(DeviceHandle);
}

void CloseDevice(const DeviceIDHelper &DeviceID)
{
  try
  {
    CloseDeviceLowLevel(DeviceID.getHandle());
  }
  catch(IgorException &)
  {
    RemoveDevice(DeviceID);
    throw;
  }

  RemoveDevice(DeviceID);
}
