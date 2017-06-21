#include "ITC_StandardHeaders.h"
#include "DeviceIDClass.h"

// Operation template: ITCSelectDevice2/Z[=number:displayErrors] number:deviceID
extern "C" int ExecuteITCSelectDevice2(ITCSelectDevice2RuntimeParamsPtr p)
{
  BEGIN_OUTER_CATCH

  // Check the validity of the device ID by constructing a deviceID object
  size_t currentDeviceID = lockToIntegerRange<size_t>(p->deviceID);
  DeviceIDHelper DeviceID(currentDeviceID);

  // Set the current thread device ID
  DeviceIDHelper::setCurrentThreadDeviceID(currentDeviceID);

  END_OUTER_CATCH
}
