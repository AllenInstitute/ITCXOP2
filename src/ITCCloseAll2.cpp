#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"

extern DeviceIDClass DeviceIDs;

// Operation template: ITCCloseAll2/Z[=number:displayErrors]

void CloseAllDevices()
{
  for(size_t currDeviceID = 0; currDeviceID < DeviceIDClass::MaxNumberOfDevices;
      currDeviceID++)
  {
    // Get the handle associated with the DeviceID
    HANDLE currDeviceHandle = nullptr;
    if(int RetVal = DeviceIDs.forceGet(currDeviceID, &currDeviceHandle))
    {
      // Slot was empty.
      // Keep going.
      continue;
    }

    try
    {
      // Turn off LED
      ITCPublicConfig lITCPublicConfig;
      ZeroMemory(&lITCPublicConfig, sizeof(lITCPublicConfig));

      ITCDLL::ITC_ConfigDevice(currDeviceHandle, &lITCPublicConfig);

      // Close the device
      ITCDLL::ITC_CloseDevice(currDeviceHandle);
      DeviceIDs.forceRemove(currDeviceID);
    }
    catch(const IgorException &)
    {
      // Keep going even if we encounter an Igor exception
      // Release the lock since we're done with it
      DeviceIDs.forceRelease(currDeviceID);
      continue;
    }
    catch(const ITCException &)
    {
      // Keep going even if we encounter a driver exception
      // Release the lock since we're done with it
      DeviceIDs.forceRelease(currDeviceID);
      continue;
    }
  }

  // Make sure that nothing is un-locked in the end.
  // Every code path above should have released the slot, but
  // due to race conditions, another thread may have picked it up
  for(size_t currDeviceID = 0; currDeviceID < DeviceIDClass::MaxNumberOfDevices;
      currDeviceID++)
  {
    DeviceIDs.forceRelease(currDeviceID);
  }
}

extern "C" int ExecuteITCCloseAll2(ITCCloseAll2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  CloseAllDevices();

  END_OUTER_CATCH
}
