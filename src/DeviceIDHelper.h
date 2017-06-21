#pragma once
#include "HelperFunctions.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// @brief Helper class for DeviceIDs
///
/// smart_ptr style implementation for getting and clearing up
/// DeviceIDs.
///
/// DeviceID is acquired when the constructor is called, and released when
/// the destructor is called.
/// The
class DeviceIDHelper
{
  static thread_local size_t threadDeviceID;

  size_t currDeviceID;
  HANDLE currDeviceHandle;

  template <typename T>
  static size_t getDeviceIDFromParameters(T *p)
  {
    // Check if the user has specified the optional deviceID
    // If they have, use that instead of the threadDeviceID
    if(p->DEVFlagEncountered)
    {
      // Update the currentDeviceID to that specified in the operation call.
      return lockToIntegerRange<size_t>(p->deviceID);
    }

    // User has not specified a particular deviceID
    // Use the threadDeviceID
    return threadDeviceID;
  }

public:
  /// @brief Open the current thread device
  ///
  /// The current thread device is the last device that was called
  /// in ITCOpenDevDevice2() or ITCSelectDevice2(),
  /// unless that device was subsequently closed with ITCCloseDevice2() or
  /// ITCCloseAll2().
  ///
  DeviceIDHelper();
  ~DeviceIDHelper();

  /// @brief Open a specified device
  ///
  /// If the /DEV=n option was specified on the command line, open
  /// DeviceID n.
  ///
  /// Otherwise, open the current thread device.
  template <typename T>
  DeviceIDHelper(T *p) : DeviceIDHelper(getDeviceIDFromParameters<T>(p)){};

  explicit DeviceIDHelper(size_t DeviceID);

  HANDLE getHandle(void) const
  {
    return currDeviceHandle;
  }

  size_t getDeviceID(void) const
  {
    return currDeviceID;
  }

  static void setCurrentThreadDeviceID(size_t DeviceID)
  {
    threadDeviceID = DeviceID;
  }

  static size_t getThreadDeviceID();

private:
  /// Prevent trying to initialise with a HANDLE
  DeviceIDHelper(const HANDLE) = delete;

  /// Delete the copy constructor
  DeviceIDHelper(const DeviceIDHelper &) = delete;

  /// Delete the assignment constructor
  DeviceIDHelper &operator=(const DeviceIDHelper &) = delete;
};
