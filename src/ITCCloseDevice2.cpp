#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"

extern DeviceIDClass DeviceIDs;

// Operation template:
// ITCCloseDevice2/Z[=number:displayErrors]/DEV=number:deviceID

extern "C" int ExecuteITCCloseDevice2(ITCCloseDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Get the deviceID and handle to use.
  DeviceIDHelper DeviceID(p);

  // Turn off LED
  ITCPublicConfig lITCPublicConfig;
  ZeroMemory(&lITCPublicConfig, sizeof(lITCPublicConfig));
  ITCDLL::ITC_ConfigDevice(DeviceID, &lITCPublicConfig);

  // Close the device handle
  ITCDLL::ITC_CloseDevice(DeviceID);
  // TODO:
  // What happens if this throws?
  // We probably need to work out if the device has actually been closed.
  // Otherwise we could end up with a zombie device (open, but not accessible
  // from Igor)

  // If we've just closed the device corresponding to the thread Device ID, then
  // set the device ID to invalid
  if(DeviceIDHelper::getThreadDeviceID() == DeviceID.getDeviceID())
  {
    DeviceIDHelper::setCurrentThreadDeviceID(
        DeviceIDClass::INVALID_DEVICE_INDEX);
  }

  // Remove the device ID
  DeviceIDs.remove(DeviceID.getDeviceID());

  END_OUTER_CATCH
}
