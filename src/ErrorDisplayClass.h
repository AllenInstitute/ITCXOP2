#pragma once

#include <string>

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

class ErrorDisplayClass
{
  enum class ErrorDisplayEnum
  {
    NotSet,
    DisplayErrors,
    SuppressErrors
  };

  /// Are error messages enabled with the /Z flag for the function?
  ErrorDisplayEnum displayErrors;

  /// Are pop-up errors enabled globally?
  /// Set by the ITCSetGlobals function.
  /// Initially set such that errors are displayed globally.
  thread_local static bool gDisplayErrors;

  /// @brief Function to parse the /Z flag
  /// @return Returns the flag status
  ///
  /// See ErrorDisplayClass::ErrorDisplayClass() for flag options.
  template <class T>
  static ErrorDisplayEnum ReadZFlag(T Params)
  {

    /// If no flag is set, the global value is used.
    /// Value if no flag is encountered
    const ErrorDisplayEnum NoFlagValue = ErrorDisplayEnum::NotSet;

    /// Display errors in the case of /Z=0
    /// Value if /Z=0 is encountered
    const ErrorDisplayEnum FlagZeroValue = ErrorDisplayEnum::DisplayErrors;

    // Suppress errors in the case of /Z or /Z!=0
    /// Value if /Z is encountered
    const ErrorDisplayEnum DefaultFlagValue = ErrorDisplayEnum::SuppressErrors;
    /// Value if /Z=k (k != 0) is encountered
    const ErrorDisplayEnum FlagNonZeroValue = ErrorDisplayEnum::SuppressErrors;

    if(!Params->ZFlagEncountered)
    {
      // No flag encountered.
      return NoFlagValue;
    }

    // Flag encountered.
    if(!Params->ZFlagParamsSet[0])
    {
      // Parameter not set.
      return DefaultFlagValue;
    }

    // Optional parameter: p->displayErrors
    if(Params->displayErrors == 0)
    {
      // Flag is set to zero
      return FlagZeroValue;
    }

    // Flag is set; value is non-zero
    return FlagNonZeroValue;
  }

public:
  /// @brief Constructs the ErrorDisplayClass
  ///
  /// Reads the operation flags and sets error display options
  /// according to whether or not the /Z flag is set.
  ///
  /// Possible flag options:
  /// [No Flag]	-- Use global error flag value
  /// /Z			-- Suppress Igor exceptions
  /// /Z=k		-- If k is zero, raise Igor exceptions.
  ///				   If k is non-zero, suppress Igor exceptions.
  ///
  /// @param Params - Parameters as passed to the function
  template <class T>
  ErrorDisplayClass(T Params)
  {
    displayErrors = ReadZFlag(Params);
  }

  /// @brief Constructs the ErrorDisplayClass
  ///
  /// Class is constructed as if the ErrorDisplay flag had not been encountered.
  ErrorDisplayClass()
  {
    displayErrors = ErrorDisplayEnum::NotSet;
  }

  /// @brief Set the global error display options
  ///
  /// See ErrorDisplayClass::ErrorDisplayClass() for flag options.
  template <class T>
  static void SetGlobalZFlag(T Params)
  {
    // Read the flag setting
    ErrorDisplayEnum ZFlagStatus = ReadZFlag(Params);

    // Update the global error display option
    switch(ZFlagStatus)
    {
    case ErrorDisplayEnum::NotSet:
      // Don't change the state of the global error display option
      return;
    case ErrorDisplayEnum::DisplayErrors:
      gDisplayErrors = true;
      return;
    case ErrorDisplayEnum::SuppressErrors:
      gDisplayErrors = false;
      return;
    default:
      // Some problem with ZFlagStatus.
      // Leave the global error display options alone.
      return;
    }
  }

  /// @brief Check if the error display options are such that errors should be
  /// shown.
  ///
  /// @return true if errors should be shown.
  bool shouldShowError();

  /// @brief Write an error message to the command window
  void WriteToCommandWindow(const std::string &message);
};
