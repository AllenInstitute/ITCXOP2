#include "ErrorDisplayClass.h"

#include "ConcurrentXOPNotice.h"
#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h

thread_local bool ErrorDisplayClass::gDisplayErrors = true;

bool ErrorDisplayClass::shouldShowError()
{
  switch(displayErrors)
  {
  case ErrorDisplayEnum::NotSet:
    // If the local flag is not set, use the global error display option
    return gDisplayErrors;

  case ErrorDisplayEnum::DisplayErrors:
    // Local flag set to display errors
    return true;

  case ErrorDisplayEnum::SuppressErrors:
    // Local flag set to not display errors
    return false;

  default:
    // Shouldn't reach here -- we've checked the 3 possible values of
    // displayErrors above
    // If in doubt, display errors
    return true;
  }
}

/// @brief Write an error message to the console
///
/// This function appends a newline after the message
void ErrorDisplayClass::WriteToCommandWindow(const std::string &message)
{
  if(shouldShowError())
  {
    XOPNotice_ts((message + "\n"));
  }
}
