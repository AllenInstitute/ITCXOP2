#pragma once
#include "ErrorDisplayClass.h"
#include <Windows.h>
#include <sstream>
#include <exception>
#include <string>
#include "itcdll.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

class IgorException : public std::exception
{
public:
  const int ErrorCode;
  const std::string Message;

  /// Constructors
  // Mark default constructor as deprecated
  // - Allows use of default constructor when a custom error code hasn't been
  // implemented
  // - Compiler warning allows us to find usages later
  __declspec(deprecated("Using default error code.  You should replace this "
                        "with a custom error code")) IgorException();
  IgorException(int errorCode);
  IgorException(int errorCode, const std::string &errorMessage);

  const char *what() const;

  /// Displays the exception if required; gets the return code.
  int HandleException(ErrorDisplayClass &ErrorDisplay) const;
};

class ITCException : public std::exception
{
  static std::string GetITCErrorMessage(HANDLE deviceHandle, DWORD errorCode,
                                        const std::string &functionName);

public:
  const DWORD ErrorCode;
  const HANDLE DeviceHandle;
  const std::string FunctionName;
  const std::string Message;

  ITCException(DWORD errorCode, HANDLE deviceHandle,
               const std::string &functionName);

  const char *what() const;

  int HandleException(ErrorDisplayClass &ErrorDisplay) const;
};

int HandleException(const std::exception e, ErrorDisplayClass &ErrorDisplay);

#define RETURN_VARIABLE "V_Value"
#define ERROR_RETURN_VARIABLE "S_errorMessage"

#define ITC_ERROR_VAR "V_ITCError"
#define XOP_ERROR_VAR "V_ITCXOPError"
#define EXCEPTION_VARS ITC_ERROR_VAR ";" XOP_ERROR_VAR

#define BEGIN_OUTER_CATCH                                                      \
  ErrorDisplayClass ErrorDisplay(p);                                           \
  SetOperationNumVar(ITC_ERROR_VAR, 0);                                        \
  SetOperationNumVar(XOP_ERROR_VAR, 0);                                        \
  try                                                                          \
  {

#define END_OUTER_CATCH                                                        \
  return 0;                                                                    \
  }                                                                            \
  catch(IgorException e)                                                       \
  {                                                                            \
    SetOperationNumVar(XOP_ERROR_VAR, (double) e.ErrorCode);                   \
    return e.HandleException(ErrorDisplay);                                    \
  }                                                                            \
  catch(ITCException e)                                                        \
  {                                                                            \
    SetOperationNumVar(ITC_ERROR_VAR, (double) e.ErrorCode);                   \
    return e.HandleException(ErrorDisplay);                                    \
  }                                                                            \
  catch(std::exception e)                                                      \
  {                                                                            \
    return HandleException(e, ErrorDisplay);                                   \
  }                                                                            \
  catch(...)                                                                   \
  {                                                                            \
    /* Unhandled exception */                                                  \
    return UNHANDLED_CPP_EXCEPTION;                                            \
  }
