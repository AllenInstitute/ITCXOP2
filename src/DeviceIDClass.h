#pragma once

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#include <memory>
#include <thread>
#include <atomic>
#include "XOPStandardHeaders.h"
#include "CustomExceptions.h"
#include "itcXOP2.h"

/// @brief Slots to store handles in.
class DeviceIDClass
{
private:
  /// @brief Threadsafe class to deal with handles
  ///
  /// Threading model is:
  /// A slot is locked to a particular thread when that thread first accesses
  /// the slot
  /// (either by storing a handle in the slot, or by reading the handle from the
  /// slot)
  ///
  /// No other thread may get the handle from that slot while it is locked to a
  /// thread
  ///
  /// The lock can be released by calling the release() function.
  class HandleSlot
  {

  private:
    std::atomic_flag deviceLocked = ATOMIC_FLAG_INIT;
    std::thread::id threadID;
    HANDLE deviceHandle;

    /// Tries to acquire the lock on the device ID.
    /// @return
    /// 0 if:
    /// - lock is successfully acquired.
    /// - lock had already been acquired in the current thread
    /// SLOT_LOCKED_TO_OTHER_THREAD if
    /// - The slot is locked to another thread
    ///
    /// NB -- this function returns an error code rather than throwing an
    /// exception.  This is because failing to acquire a lock on a slot
    /// is sometimes expected.  In particular, in trySetHandle() it's OK
    /// to not be able to store a handle in a particular slot.
    int tryAcquireLock() noexcept
    {
      if(deviceLocked.test_and_set())
      {
        // Device is locked to a thread.
        // [deviceLocked was true before we set it]

        // Make sure the device is locked to the current thread.
        if(threadID == std::this_thread::get_id())
        {
          // Locked thread ID matches the current thread.
          // All is well
          return 0;
        }
        else
        {
          // Device is locked to a thread that is not the current thread.
          // Can't acquire the device.
          return SLOT_LOCKED_TO_OTHER_THREAD;
        }
      }
      else
      {
        // Device is not locked to a thread
        // [deviceLocked was false before we set it]

        // Lock has been acquired.
        // Update the lock so that it points to this thread

        // There ought not to be a race condition here:
        // - Only one thread can acquire the lock from free at a time (it's an
        // atomic flag)
        // - This thread is currently busy running this function, so no other
        // function can be accessing the deviceID.
        //   (if it weren't busy, then it's possible that another function would
        //   see (in error) that the
        //      device was locked, but not to this thread)
        // - If another thread come along and tries to acquire the lock, it'll
        // see that it's locked and
        //     the thread id won't match the other thread's id in any case
        //     (it'll be the id that doesn't match any thread).
        threadID = std::this_thread::get_id();
        return 0;
      }
    }

  public:
    HandleSlot() : threadID(), deviceHandle(nullptr){};

    /// @brief Get a handle from the slot
    /// That slot is automatically locked to the current thread.
    ///
    /// @return devHandle -- the device handle stored in that slot
    ///
    /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) -- The slot is
    ///   locked to another thread, and can't be accessed from this one.
    ///
    /// @throw IgorException(SLOT_EMPTY) -- The slot doesn't have a handle
    ///   stored in it
    ///
    HANDLE getHandle()
    {
      // Quickly test if the slot is currently empty, without locking it
      if(deviceHandle == nullptr)
      {
        // The slot was empty when this function was called
        throw IgorException(SLOT_EMPTY);
      }

      // Acquire the lock
      if(int ErrorCode = tryAcquireLock())
      {
        // Acquiring the lock gave an error.
        // Throw this error.
        throw IgorException(ErrorCode);
      }

      if(deviceHandle == nullptr)
      {
        // The slot is empty
        // Empty slots shouldn't be locked
        releaseLock();
        throw IgorException(SLOT_EMPTY);
      }

      return deviceHandle;
    }

    /// @brief Try to store a handle in this slot
    /// @return @c true if the handle was stored successfully
    /// @c false otherwise.
    ///
    /// If the handle was not stored successfully, the calling function
    /// should try storing the handle in a different slot.
    ///
    bool trySetHandle(HANDLE HandleToStore) noexcept
    {
      // Peek if the slot is empty first
      if(deviceHandle != nullptr)
      {
        // Slot is not empty
        // We didn't manage to set the value.
        return false;
      }

      // The slot is empty.
      // Check the lock
      // If the slot is empty, then ordinarily the lock should not be set
      // HOWEVER -- there are race conditions:
      // - The slot may be empty, but the lock may still be held by some other
      // thread (see release())
      // - The slot may be acquired by another thread between testing above and
      // acquiring the lock here.
      // We'll fail in the event that one of these race conditions occurs.
      //
      // All this means is that we end up using a different slot.
      // This could theoretically cause a problem if we don't find another slot
      // (even though one was actually free)
      // In practice, I don't think this is worth worrying about.
      if(int RetVal = tryAcquireLock())
      {
        // Acquiring the lock gave an error
        // We can't set the handle in this thread.

        // We didn't manage to set the value.
        return false;
      }

      // The device slot is now (or already was) locked to the current thread.
      // Check again that the slot is empty
      // The slot could be non-empty if another thread managed to acquire and
      // release the slot
      // in between checking the devHandle above, and acquiring the lock.

      // Note that holding onto the lock while checking devHandle could lead to
      // issues:
      //
      // Conisder the case of 3 threads, A, B, C.
      // Thread A initially has the device.  We want to pass control of the
      // device to Thread C.
      //
      // Thread A releases the device.
      // Thread B calls ITCOpenDevice2(), which blocks the slot while it checks
      // if it's empty.
      // Thread C is informed by thread A that it can have the device.  It tries
      // to use it, but finds the slot
      //   is blocked by Thread B.
      //
      // I'm not sure if there's a nice way around this problem -- we need to
      // have a hold of the slot to make
      // sure that no other threads change the value in it between testing and
      // setting.
      if(deviceHandle != nullptr)
      {
        // The slot is not currently empty.

        // We're not going to be using this slot.
        // We know that this thread was unlocked before the call to
        // tryAcquireLock(), so we should release the lock.
        //
        // How do we know it was unlocked?
        //
        // There are 3 states the slot could have been in, prior to the call to
        // tryAcquireLock():
        // - Locked to another thread
        // - Locked to this thread
        // - Unlocked
        //
        // We know that the first two conditions cannot be true:
        //
        // - If another thread had the lock, we would not have managed to
        // acquire the lock in tryAcquireLock()
        //
        // and
        //
        // - If this thread had held the lock when calling tryAcquireLock(),
        // then it must have held the lock
        //     when entering this function
        //     -- no other thread could have taken the lock
        //     -- this thread couldn't have released it, since it was busy in
        //     this function
        // - If it held the lock when entering this function, then the value of
        // deviceHandle could not have been changed
        //     -- the slot was locked to this thread, so no other thread could
        //     have changed it
        //     -- this thread could not have changed it, since it was busy in
        //     this function
        // - But the value of devHandle *did* change between checking it at the
        // start of this function, and now
        // - Therefore, this thread can't have held the lock upon entering this
        // function.
        //
        // Since the first two conditions cannot be true, the 3rd condition must
        // be true (the slot was unlocked)
        // The slot was unlocked before the call to tryAcquireLock(), we should
        // return it to the unlocked state.
        releaseLock();

        // We didn't manage to set the value.
        return false;
      }

      // Slot is empty.
      // Don't need to worry about race conditions now because we've locked the
      // slot to this thread,
      // in the call to tryAcquireLock().
      // Set the device handle
      deviceHandle = HandleToStore;
      return true;
    }

    /// @brief Empties and unlocks a slot
    ///
    /// The slot may only be cleared if:
    /// - It is not locked to another thread.
    /// - It is not currently empty
    ///
    /// @throw IgorException(SLOT_EMPTY) -- The slot was already empty.
    /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) -- The slot is
    ///   locked to another thread.
    ///
    void clearHandle()
    {
      // Peek if the slot is empty when this function is called
      if(deviceHandle == nullptr)
      {
        throw IgorException(SLOT_EMPTY);
      }

      // Get the lock
      if(int RetVal = tryAcquireLock())
      {
        // Acquiring the lock gave an error
        throw IgorException(RetVal);
      }

      // Check that the slot actually had something in it
      if(deviceHandle == nullptr)
      {
        throw IgorException(SLOT_EMPTY);
      }

      // Clear the handle
      deviceHandle = nullptr;

      // Release the lock
      // (allow the slot to be re-used)
      releaseLock();
    }

    /// @brief Releases the lock associated with this slot
    ///
    /// The lock may only be released if it is not locked to another thread.
    ///
    /// If the slot isn't locked to a thread then, the slot will briefly be
    /// locked to the current thread
    /// before being released.
    ///
    /// This function doesn't care if the slot was empty
    /// This allows it to be called after removing a device.
    ///
    /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) if the slot was locked
    /// to another thread
    void releaseLock()
    {
      // Check the lock
      if(int RetVal = tryAcquireLock())
      {
        // Acquiring the lock gave an error
        throw IgorException(RetVal);
      }

      // We have the lock, so we're allowed to release it
      // Clear the thread ID

      // There ought not be a race condition here:
      // - This thread is currently executing this function, so no other
      // function on this thread can be accessing the slot
      //     (if it weren't busy, then it's possible that another function would
      //     see (in error) that the
      //      slot was locked, but not to this thread)
      // - Any other thread will see that the slot is locked, but the thread id
      // won't correspond to any thread
      //    (see default ctor for std::thread::id). This is valid behaviour,
      //    since the release() function
      //    in this thread hasn't returned yet.
      threadID = std::thread::id();

      // Open the lock
      deviceLocked.clear();
    }

    /// @brief Force getting a handle, regardless of lock.
    ///
    /// @return Handle associated with slot.
    /// NB -- this handle may be nullptr.
    /// Calling functions must check the handle before use!
    ///
    /// Makes an best-effort attempt to lock the handle to the current
    /// thread, regardless of which thread currently has the lock.
    ///
    /// This function DOES NOT respect the threading model
    /// There will likely be race conditions.
    ///
    /// In particular:
    /// There is a race condition when this function is called at the
    /// same time as tryAcquireLock.  It may be that, upon exiting
    /// this function, the threadID is equal to that of the other thread
    /// calling tryAcquireLock().
    ///
    /// Similarly this function may cause tryAcquireLock() to return that
    /// the thread is locked to the other thread, when the threadID matches
    /// this thread.
    ///
    /// If another thread thinks it has the lock, that other thread will
    /// be able to change the handle or release the lock, despite the fact
    /// that the threadID suggests that the slot is locked to this thread.
    ///
    /// In summary, be careful with this function.  Only use it when you
    /// don't mind breaking the threading model.
    ///
    /// FINALLY:
    /// This function behaves properly if no other threads are currently using
    /// any ITC functions.
    HANDLE forceGetHandle() noexcept
    {
      // Set the device as locked to the current thread.
      deviceLocked.test_and_set();

      // ****** Race condition happens here ******

      threadID = std::this_thread::get_id();
      // Assuming the race condition didn't happen above,
      // the slot is locked to the current thread.
      //
      // Any other function which tries to acquire the lock will now not
      // be able to.
      //
      // However, given that we didn't acquire the lock in the usual way,
      // it's possible that another thread also thinks it has the lock.

      return deviceHandle;
    }

    /// @brief Force releasing a lock, regardless of which thread it's locked to
    void forceReleaseLock() noexcept
    {
      /// Clear the thread ID
      threadID = std::thread::id();

      /// Release the lock
      deviceLocked.clear();
    }

    /// @brief Force deleting a thread ID
    void forceClearHandle() noexcept
    {

      // Set flag and clear threadID
      // (other threads will see that the thread is locked, but not to them)
      deviceLocked.test_and_set();
      // *** Race condition here ***
      threadID = std::thread::id();

      // clear the device handle
      deviceHandle = nullptr;

      /// Release the lock
      deviceLocked.clear();
    }
  };

public:
  static const size_t MaxNumberOfDevices = 16 * 4;

private:
  HandleSlot handleArray[MaxNumberOfDevices];

  /// @brief Check if a deviceID is valid to use
  /// @throw IgorException(SLOT_EMPTY) -- Invalid deviceID
  void CheckDeviceID(size_t DeviceID)
  {
    // Ensure we don't read past the end of the array
    if(DeviceID >= MaxNumberOfDevices)
    {
      throw IgorException(SLOT_EMPTY, "Device ID out of range");
    }
  }

public:
  static const double INVALID_DEVICE_ID;
  static const size_t INVALID_DEVICE_INDEX;

  DeviceIDClass()
  {
    // Do nothing in constructor
  }

  /// @brief Store a handle in the list.
  ///
  /// @param[in] h Handle to store
  /// @return DeviceID Assigned Device ID
  ///
  /// The assigned device ID will be locked to the current thread.
  ///
  /// @throw IgorException(COULDNT_FIND_EMPTY_SLOT) -- couldn't find an empty
  /// slot to store the handle in
  size_t store(HANDLE h)
  {
    for(size_t currDeviceID = 0; currDeviceID < MaxNumberOfDevices;
        currDeviceID++)
    {
      // Store the handle in the first empty slot
      if(handleArray[currDeviceID].trySetHandle(h))
      {
        return currDeviceID;
      }
    }

    // Couldn't find a slot.
    throw IgorException(COULDNT_FIND_EMPTY_SLOT);
  };

  /// @brief Convert DeviceID into handle.
  ///
  /// Obtain the handle associated with a device ID.
  ///
  /// @param	DeviceID	The Device ID to get the handle for.
  /// @return	DeviceHandle	The handle associated with the DeviceID
  ///
  /// @throw IgorException(SLOT_EMPTY) -- DeviceID does not correspond to an
  /// open device
  /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) -- The DeviceID is
  ///   currently locked to another thread, and cannot be accessed from this
  ///   thread
  HANDLE get(size_t DeviceID)
  {
    CheckDeviceID(DeviceID);

    // Get the handle
    return handleArray[DeviceID].getHandle();
  };

  /// Convenience overload
  HANDLE get(double DeviceID)
  {
    return get((size_t) DeviceID);
  }

  /// @brief Clear the handle associated with a DeviceID
  ///
  /// Further attempts to access that DeviceID will cause an error
  ///   (unless the DeviceID is re-used)
  ///
  /// @param	DeviceID  DeviceID to remove.
  ///
  /// @return	Igor error code
  /// @throw IgorException(SLOT_EMPTY) -- DeviceID doesn't correspond to an open
  /// device
  /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) -- Cannot access the
  /// slot from this thread
  void remove(size_t DeviceID)
  {
    // Check that the device ID is valid
    CheckDeviceID(DeviceID);

    // Clear the slot
    handleArray[DeviceID].clearHandle();
  };

  /// Convenience overload
  void remove(double deviceID)
  {
    remove((size_t) deviceID);
  }

  /// @brief Release the current thread's lock on a device ID.
  ///
  /// @throw IgorException(SLOT_EMPTY) - deviceID doesn't correspond to an open
  /// device
  /// @throw IgorException(SLOT_LOCKED_TO_OTHER_THREAD) - Slot is locked to
  /// another thread
  void release(size_t deviceID)
  {
    // Check the deviceID is valid
    CheckDeviceID(deviceID);

    // Release the lock
    handleArray[deviceID].releaseLock();
  }

  /// Convenience overload
  void release(double deviceID)
  {
    release(size_t(deviceID));
  }

  /// @brief Force getting a deviceID.
  ///
  /// Use *only* in function CloseAll2()
  ///
  /// Doesn't throw exceptions
  /// @return Igor error code
  int forceGet(size_t deviceID, HANDLE *handle) noexcept
  {
    if(handle == nullptr)
    {
      return USING_NULL_REFVAR;
    }

    if(deviceID >= MaxNumberOfDevices)
    {
      return SLOT_EMPTY;
    }

    *handle = handleArray[deviceID].forceGetHandle();

    if(*handle == nullptr)
    {
      handleArray[deviceID].forceReleaseLock();
      return SLOT_EMPTY;
    }

    return 0;
  }

  /// @brief Force releasing the lock on a deviceID slot.
  ///
  /// Use *only* in function CloseAll2()
  ///
  /// Doesn't throw exceptions
  /// @return Igor error code
  int forceRelease(size_t deviceID) noexcept
  {
    if(deviceID >= MaxNumberOfDevices)
    {
      return SLOT_EMPTY;
    }

    handleArray[deviceID].forceReleaseLock();

    return 0;
  }

  /// @brief Force clearing a deviceID slot.
  ///
  /// Use *only* in function CloseAll2()
  ///
  /// Doesn't throw exceptions
  /// @return Igor error code
  int forceRemove(size_t deviceID) noexcept
  {
    if(deviceID >= MaxNumberOfDevices)
    {
      return SLOT_EMPTY;
    }

    handleArray[deviceID].forceClearHandle();

    return 0;
  }
};
