#include "CustomExceptions.h"
#include "itcXOP2.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

#include "itcdll.h"

//--------------------------------------------------------------
// IgorException
//--------------------------------------------------------------

IgorException::IgorException(int errorCode) : m_errorCode(errorCode)
{
  if(debuggingEnabled)
  {
    DebugOut("IgorException", fmt::to_string(*this));
  }
}

IgorException::IgorException(int errorCode, const std::string &errorMessage)
    : m_errorCode(errorCode), m_message(errorMessage)
{
  if(debuggingEnabled)
  {
    DebugOut("IgorException", fmt::to_string(*this));
  }
}

const char *IgorException::what() const
{
  return m_message.c_str();
}

int IgorException::HandleException(ErrorDisplayClass &ErrorDisplay) const
{
  if(ErrorDisplay.shouldShowError())
  {
    ErrorDisplay.WriteToCommandWindow(m_message);

    return m_errorCode;
  }
  else
  {
    return 0;
  }
}

const DWORD IgorException::GetErrorCode() const
{
  return m_errorCode;
}

const std::string IgorException::GetMessage() const
{
  return m_message;
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
    : m_errorCode(errorCode), m_deviceHandle(deviceHandle),
      m_functionName(functionName)
{
  if(debuggingEnabled)
  {
    DebugOut("ITCException", fmt::to_string(*this));
  }
}

int ITCException::HandleException(ErrorDisplayClass &ErrorDisplay) const
{
  if(ErrorDisplay.shouldShowError())
  {
    ErrorDisplay.WriteToCommandWindow(GetMessage());

    return ITC_DLL_ERROR;
  }
  else
  {
    return 0;
  }
}

const DWORD ITCException::GetErrorCode() const
{
  return m_errorCode;
}

const std::string ITCException::GetFunctionName() const
{
  return m_functionName;
}

const HANDLE ITCException::GetDeviceHandle() const
{
  return m_deviceHandle;
}

const std::string ITCException::GetMessage() const
{
  return GetITCErrorMessage(m_deviceHandle, m_errorCode, m_functionName);
}

const char *ITCException::what() const
{
  return GetMessage().c_str();
}

//--------------------------------------------------------------
// std::exception
//--------------------------------------------------------------

int HandleException(const std::exception e, ErrorDisplayClass &ErrorDisplay)
{
  ErrorDisplay.WriteToCommandWindow(
      "Encountered unhandled C++ exception during XOP execution.");
  ErrorDisplay.WriteToCommandWindow(e.what());

  return UNHANDLED_CPP_EXCEPTION;
}
