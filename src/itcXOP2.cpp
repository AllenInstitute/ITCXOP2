#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h

#include "cmake_config.h"
#include "itcXOP2.h"
#include "DeviceIDClass.h"
#include "RegisterOperations.h"
#include "ConcurrentXOPNotice.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/*	XOPEntry()

  This is the entry point from the host application to the XOP for all
  messages after the INIT message.
*/
extern "C" void XOPEntry(void)
{
  XOPIORecResult result = 0;

  switch(GetXOPMessage())
  {
  case NEW:
  case CLEANUP:
    // Close all devices when opening a new experiment, or when closing an
    // experiment.
    try
    {
      CloseAllDevices();
    }
    catch(...)
    {
      DebugOut("XOPEntry", "Ignoring unknown exception");
    }
    break;
  case IDLE:
    OutputQueuedNotices();
    break;
  }
  SetXOPResult(result);
}

/*	XOPMain(ioRecHandle)

  This is the initial entry point at which the host application calls XOP.
  The message sent by the host must be INIT.

  XOPMain does any necessary initialization and then sets the XOPEntry field of
  the
  ioRecHandle to the address to be called for future messages.
*/
HOST_IMPORT int XOPMain(IORecHandle ioRecHandle)
{
  int result;

  XOPInit(ioRecHandle);  // Do standard XOP initialization.
  SetXOPEntry(XOPEntry); // Set entry point for future calls.
  SetXOPType(RESIDENT | IDLES);

  if(igorVersion < XOP_MINIMUM_IGORVERSION)
  {                         // Check Igor version
    SetXOPResult(OLD_IGOR); // OLD_IGOR is defined in itcXOP2.h and there are
                            // corresponding error strings in itcXOP2.r and
                            // itcXOP2WinCustom.rc.
    return EXIT_FAILURE;
  }

  if(result = RegisterOperations())
  {
    SetXOPResult(result);
    return EXIT_FAILURE;
  }

  SetXOPResult(0);
  return EXIT_SUCCESS;
}
