#include "CustomExceptions.h"
#include "itcXOP2.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#include "itcdll.h"

//--------------------------------------------------------------
// IgorException
//--------------------------------------------------------------

IgorException::IgorException(int errorCode) : ErrorCode(errorCode)
{
  DebugOut("IgorException", fmt::to_string(*this));
}

IgorException::IgorException(int errorCode, const std::string &errorMessage)
    : ErrorCode(errorCode), Message(errorMessage)
{
  DebugOut("IgorException", fmt::to_string(*this));
}

const char *IgorException::what() const
{
  return Message.c_str();
}

int IgorException::HandleException(ErrorDisplayClass &ErrorDisplay) const
{
  if(Message.size() > 0)
  {
    ErrorDisplay.WriteToCommandWindow(Message);
  }

  if(ErrorDisplay.shouldShowError())
  {
    return ErrorCode;
  }
  else
  {
    return 0;
  }
}

//--------------------------------------------------------------
// ITCException
//--------------------------------------------------------------
std::string ITCException::GetITCErrorMessage(HANDLE deviceHandle,
                                             DWORD errorCode,
                                             const std::string &functionName)
{
  // Get a nicely formatted ITC error message
  const int TextBufferSize = 256;
  char TextBuffer[TextBufferSize];

  // Attempt to get the error message from the ITCDLL
  try
  {
    ITCDLL::ITC_GetStatusText(deviceHandle, errorCode, TextBuffer,
                              TextBufferSize);
  }
  catch(const ITCException &)
  {
    // Error in call to ITC_GetStatusText.
    // No nicely formatted status text available.
    // Just display the function name and the return value.
    return fmt::format(
        FMT_STRING("Error in call to {}.\rReported error: {:#X}"), functionName,
        errorCode);
  }

  // Retrieved the status text. Display the nicely formatted error message.
  return fmt::format(
      FMT_STRING("DLL Error.\rError in call to {}.\rReported error: {}"),
      functionName, TextBuffer);
}

ITCException::ITCException(DWORD errorCode, HANDLE deviceHandle,
                           const std::string &functionName)
    : ErrorCode(errorCode), DeviceHandle(deviceHandle),
      FunctionName(functionName),
      Message(GetITCErrorMessage(deviceHandle, ErrorCode, functionName))
{
  DebugOut("ITCException", fmt::to_string(*this));
}

int ITCException::HandleException(ErrorDisplayClass &ErrorDisplay) const
{
  ErrorDisplay.WriteToCommandWindow(Message);

  if(ErrorDisplay.shouldShowError())
  {
    return ITC_DLL_ERROR;
  }
  else
  {
    return 0;
  }
}

const char *ITCException::what() const
{
  return Message.c_str();
}

//--------------------------------------------------------------
// std::exception
//--------------------------------------------------------------

int HandleException(const std::exception e, ErrorDisplayClass &ErrorDisplay)
{
  ErrorDisplay.WriteToCommandWindow(
      "Encountered unhandled C++ exception during XOP execution.");
  if(strlen(e.what()) > 0)
  {
    ErrorDisplay.WriteToCommandWindow(e.what());
  }

  return UNHANDLED_CPP_EXCEPTION;
}
