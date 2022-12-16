#pragma once
#include "ErrorDisplayClass.h"
#include <Windows.h>
#include <exception>
#include <string>
#include "itcdll.h"
#include "fmt/format.h"
#include "fmt/ranges.h"
#include "fmt/chrono.h"

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

class IgorException : public std::exception
{
  const int m_errorCode;
  const std::string m_message;

public:
  /// Constructors
  IgorException(int errorCode);
  IgorException(int errorCode, const std::string &errorMessage);

  const char *what() const;

  /// Displays the exception if required; gets the return code.
  int HandleException(ErrorDisplayClass &ErrorDisplay) const;

  const DWORD GetErrorCode() const;
  const std::string GetMessage() const;
};

template <>
struct fmt::formatter<IgorException> : fmt::formatter<std::string>
{
  // parse is inherited from formatter<std::string>.
  template <typename FormatContext>
  auto format(const IgorException &e, FormatContext &ctx)
  {
    return format_to(ctx.out(), FMT_STRING("error code: {:#X}, what: {}"),
                     e.GetErrorCode(), e.GetMessage());
  }
};

class ITCException : public std::exception
{
  static std::string GetITCErrorMessage(HANDLE deviceHandle, DWORD errorCode,
                                        const std::string &functionName);

  const DWORD m_errorCode;
  const HANDLE m_deviceHandle;
  const std::string m_functionName;

public:
  ITCException(DWORD errorCode, HANDLE deviceHandle,
               const std::string &functionName);

  const char *what() const;

  int HandleException(ErrorDisplayClass &ErrorDisplay) const;

  const DWORD GetErrorCode() const;
  const std::string GetFunctionName() const;
  const HANDLE GetDeviceHandle() const;
  const std::string GetMessage() const;
};

template <>
struct fmt::formatter<ITCException> : fmt::formatter<std::string>
{
  // parse is inherited from formatter<std::string>.
  template <typename FormatContext>
  auto format(const ITCException &e, FormatContext &ctx)
  {
    return format_to(
        ctx.out(),
        FMT_STRING(
            "error code: {:#X}, device handle: {}, function: {}, message: {}"),
        e.GetErrorCode(), e.GetDeviceHandle(), e.GetFunctionName(),
        e.GetMessage());
  }
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
  catch(const IgorException &e)                                                \
  {                                                                            \
    SetOperationNumVar(XOP_ERROR_VAR, (double) e.GetErrorCode());              \
    return e.HandleException(ErrorDisplay);                                    \
  }                                                                            \
  catch(const ITCException &e)                                                 \
  {                                                                            \
    SetOperationNumVar(ITC_ERROR_VAR, (double) e.GetErrorCode());              \
    return e.HandleException(ErrorDisplay);                                    \
  }                                                                            \
  catch(const std::exception &e)                                               \
  {                                                                            \
    return HandleException(e, ErrorDisplay);                                   \
  }                                                                            \
  catch(...)                                                                   \
  {                                                                            \
    /* Unhandled exception */                                                  \
    return UNHANDLED_CPP_EXCEPTION;                                            \
  }
