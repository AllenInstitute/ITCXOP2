#include "CustomExceptions.h"
#include "DeviceIDHelper.h"
#include "DeviceIDClass.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

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
