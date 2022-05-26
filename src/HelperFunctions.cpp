#include "HelperFunctions.h"
#include "itcXOP2.h"
#include "itcdll.h"
#include "git_version.h"
#include "fmt/format.h"
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cmath>

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

using namespace fmt::literals;

thread_local bool debuggingEnabled = false;

std::string getStringFromHandle(Handle strHandle)
{
  // Check for special case of null handle.
  if(strHandle == nullptr)
  {
    return "";
  }

  size_t strLen = GetHandleSize(strHandle);
  return std::string(*strHandle, strLen);
}

/// @brief Set dimension labels on wave
///
/// @param	h	Wave to set dimension labels on
/// @param	Dimension	Dimension to set labels on
/// @param	colLabels	vector of labels to set
///
/// dimLabels[k] will be assigned to index k of the wave
///
/// @return Igor error code
void SetDimensionLabels(waveHndl h, int Dimension,
                        const std::vector<std::string> &dimLabels)
{
  // Check wave exists.
  if(h == nullptr)
  {
    throw IgorException(NULL_WAVE_OP);
  }

  for(size_t k = 0; k < dimLabels.size(); k++)
  {
    if(dimLabels[k].size() == 0)
    {
      // Empty string.  Skip.
      continue;
    }

    if(int RetVal = MDSetDimensionLabel(h, Dimension, k, dimLabels[k].c_str()))
    {
      throw IgorException(RetVal);
    }
  }
}

std::vector<size_t> checkWave_2D(waveHndl waveH, int dataType,
                                 size_t minNumRows)
{
  // Check wave exists
  if(waveH == nullptr)
  {
    throw IgorException(NULL_WAVE_OP);
  }

  // check wave types
  const int currWaveType = WaveType(waveH);
  if(currWaveType != dataType)
  {
    throw IgorException(NT_INCOMPATIBLE);
  }

  // Get the dimension sizes
  int numDimensions;
  CountInt dimensionSizes[MAX_DIMENSIONS + 1];
  if(int RetVal = MDGetWaveDimensions(waveH, &numDimensions, dimensionSizes))
  {
    throw IgorException(RetVal);
  }

  // Number of rows should be at least minimum number of rows.
  size_t numRows = static_cast<size_t>(dimensionSizes[ROWS]);
  if(numRows < minNumRows)
  {
    throw IgorException(NEED_MIN_ROWS);
  }

  // Number of layers must be 0 (2D wave), since the indexing assumes a 2D wave
  if(dimensionSizes[LAYERS] != 0)
  {
    throw IgorException(EXPECT_MATRIX);
  }

  // Number of columns is the number of channels to read.
  size_t numCols =
      std::max(static_cast<size_t>(dimensionSizes[COLUMNS]), (size_t) 1);

  std::vector<size_t> dimSizes = {numRows, numCols};
  return dimSizes;
}

size_t getOffset_2D(size_t Row, size_t Col, const std::vector<size_t> &dimSizes)
{
  return Col * dimSizes[ROWS] + Row;
}

template <>
bool lockToIntegerRange<bool>(double val)
{
  // If value is NaN or inf, return an appropriate error.
  if(std::isnan(val) || std::isinf(val))
  {
    throw IgorException(kDoesNotSupportNaNorINF);
  }

  return std::abs(val) > 1e-8;
}

std::size_t GetWaveElementSize(int dataType)
{
  switch(dataType)
  {
  /// size assumptions about real/double values are
  // not guaranteed by the standard but should work
  case NT_CMPLX | NT_FP32:
    return 2 * sizeof(float);
    break;
  case NT_CMPLX | NT_FP64:
    return 2 * sizeof(double);
    break;
  case NT_FP32:
    return sizeof(float);
    break;
  case NT_FP64:
    return sizeof(double);
    break;
  case NT_I8:
    return sizeof(int8_t);
    break;
  case NT_I8 | NT_UNSIGNED:
    return sizeof(uint8_t);
    break;
  case NT_I16:
    return sizeof(int16_t);
    break;
  case NT_I16 | NT_UNSIGNED:
    return sizeof(uint16_t);
    break;
  case NT_I32:
    return sizeof(int32_t);
    break;
  case NT_I32 | NT_UNSIGNED:
    return sizeof(uint32_t);
    break;
  case NT_I64:
    return sizeof(int64_t);
    break;
  case NT_I64 | NT_UNSIGNED:
    return sizeof(uint64_t);
    break;
  default:
    return 0;
  }
}

void WaveClear(waveHndl wv)
{
  if(wv == nullptr)
  {
    throw IgorException(USING_NULL_REFVAR);
  }

  const auto numBytes = WavePoints(wv) * GetWaveElementSize(WaveType(wv));

  if(numBytes == 0) // nothing to do
  {
    return;
  }

  MemClear(WaveData(wv), numBytes);
}

void SetOperationReturn(const std::string &name, const std::string &value)
{
  if(int err = SetOperationStrVar(name.c_str(), value.c_str()))
  {
    throw IgorException(err, fmt::format("Error setting {}.", name));
  }
}

void SetOperationReturn(const std::string &name, double value)
{
  if(int err = SetOperationNumVar(name.c_str(), value))
  {
    throw IgorException(err, fmt::format("Error setting {}.", name));
  }
}

StrStrMap GetVersionInfo(const std::string &xopName)
{
  std::map<std::string, std::string> m;

  m["name"]    = xopName;
  m["version"] = GIT_REVISION;

#ifdef MACIGOR64
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdate-time"
#endif

  m["builddate"] = "{} {}"_format(__DATE__, __TIME__); // NOLINT

#ifdef MACIGOR64
#pragma clang diagnostic pop
#endif

#ifdef MACIGOR64
  m["compiler"] = "Clang {}.{}.{}"_format(__clang_major__, __clang_minor__,
                                          __clang_patchlevel__);
#else
  m["compiler"] = "Visual Studio {}"_format(_MSC_VER);
#endif

  return m;
}
