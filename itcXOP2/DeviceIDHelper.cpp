#include "CustomExceptions.h"
#include "DeviceIDHelper.h"
#include "DeviceIDClass.h"

const double DeviceIDClass::INVALID_DEVICE_ID    = -1;
const size_t DeviceIDClass::INVALID_DEVICE_INDEX = (size_t) -1;

thread_local size_t DeviceIDHelper::threadDeviceID =
    DeviceIDClass::INVALID_DEVICE_INDEX;

DeviceIDClass DeviceIDs;

DeviceIDHelper::DeviceIDHelper(size_t DeviceID)
{
  currDeviceID     = DeviceID;
  currDeviceHandle = DeviceIDs.get(currDeviceID);
}
DeviceIDHelper::DeviceIDHelper() : DeviceIDHelper(getThreadDeviceID())
{
}

DeviceIDHelper::~DeviceIDHelper()
{
  DeviceIDs.release(currDeviceID);
}

size_t DeviceIDHelper::getThreadDeviceID()
{
  if(threadDeviceID == DeviceIDClass::INVALID_DEVICE_INDEX)
  {
    throw IgorException(THREAD_DEVICE_ID_NOT_SET);
  }

  return threadDeviceID;
}
