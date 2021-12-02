#pragma once

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

template <typename T>
bool ReadConvertVoltageFlag(T *p)
{
  if(p->VFlagEncountered)
  {
    if(p->VFlagParamsSet[0])
    {
      // Optional parameter: p->voltageOn
      // Case of /V=n
      // Convert value if n=1, disable if n=0
      return lockToIntegerRange<bool>(p->voltageOn);
    }

    // Case of /V
    // -> Convert value
    return true;
  }

  // No flag encountered
  // -> Convert value
  return true;
}

template <typename T>
bool ReadCalibrateFlag(T *p)
{
  if(p->CFlagEncountered)
  {
    if(p->CFlagParamsSet[0])
    {
      // Optional parameter: p->calibrateOn
      // Case of /C=n
      // Enable calibration if n=1, disable if n=0
      return lockToIntegerRange<bool>(p->calibrateOn);
    }

    // Case of /C
    // -> Enable calibration
    return true;
  }

  // No flag encountered
  // -> Enable calibration by default.
  return true;
}
