#pragma once
#define NOMINMAX

#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "ErrorDisplayClass.h"
#include "CustomExceptions.h"
#include <string>
#include <vector>
#include <functional>

// Usign std::min/max
#undef min
#undef max

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

extern thread_local bool debuggingEnabled;

template <class T>
void ReadGlobalDebugFlag(T *p)
{
  if(!p->DFlagEncountered)
    return;

  if(!p->DFlagParamsSet[0])
  {
    debuggingEnabled = true;
    return;
  }

  debuggingEnabled = lockToIntegerRange<bool>(p->debugOutput);
}

/// @brief Converts a double value to a specified integer type.
///
/// Returns an error if:
/// - The value is NaN of +/- inf
/// - The value lies outside the range of the integer representation.
///
/// The value is truncated towards zero
/// That is:
/// - Positive numbers are rounded down
/// - Negative numbers are rounded up
///
/// If the value is NaN, zero is returned.
///
/// @tparam	T	integer type to convert to.
/// @param	val	value to convert
/// @return	converted value
template <typename T>
T lockToIntegerRange(double val)
{
  // If value is NaN or inf, return an appropriate error.
  if(std::isnan(val) || std::isinf(val))
  {
    throw IgorException(kDoesNotSupportNaNorINF);
  }

  // If value lies outside range of integer type, return an error.
  if(val > (double) std::numeric_limits<T>::max() ||
     val < (double) std::numeric_limits<T>::min())
  {
    throw IgorException(kParameterOutOfRange);
  }

  // Truncate towards zero.
  // 10.1 becomes 10
  // -10.1 becomes -10.
  if(val > 0)
  {
    val = std::floor(val);
  }
  if(val < 0)
  {
    val = std::ceil(val);
  }

  return static_cast<T>(val);
}

template <>
bool lockToIntegerRange<bool>(double val);

/// @brief Return the size in bytes of the given Igor Pro wave types
///
/// The returned size is zero for non-numeric wave types
std::size_t GetWaveElementSize(int dataType);

/// @brief Set all elements of the given wave to zero
///
/// Does nothing for non-numeric waves
void WaveClear(waveHndl wv);

/// @brief Convert Igor string into std::string
///
/// If the string handle is null, the empty string is returned.
///
/// @param	strHandle	handle to Igor string
/// @return	std::string containing the same data
std::string getStringFromHandle(Handle strHandle);

void SetDimensionLabels(waveHndl destWaveH, int Dimension,
                        const std::vector<std::string> &dimLabels);

/// Need to call FinishDestWave after modifying data
template <typename T>
void MakeDestWave(T p, waveHndl *destWaveH, int dataType, const int numRows,
                  const int numCols)
{
  if(destWaveH == nullptr)
  {
    throw IgorException(USING_NULL_REFVAR);
  }

  // Require that we have a destination wave.
  // [Could modify to have a default output wave?]
  if(!(p->destEncountered))
  {
    // TODO: custom error code
    throw IgorException(EXIT_FAILURE);
  }

  // Read the options
  int options = 0;
  if(p->FREEFlagEncountered)
  {

    // Only allow /FREE flag if called from function
    if(!p->calledFromFunction && !p->tp)
    {
      throw IgorException(BADFLG);
    }

    options |= kOpDestWaveMakeFreeWave;
  }
  if(p->OFlagEncountered)
  {
    options |= kOpDestWaveOverwriteOK;
  }
  options |= kOpDestWaveOverwriteExistingWave;

  // Set the dimension sizes
  CountInt dimensionSizes[MAX_DIMENSIONS + 1];
  MemClear(dimensionSizes, sizeof(dimensionSizes));
  dimensionSizes[ROWS]    = numRows;
  dimensionSizes[COLUMNS] = numCols;

  int destWaveCreated;

  if(int RetVal = GetOperationDestWave(
         p->dest.dfH, p->dest.name, p->destParamsSet[0], options,
         dimensionSizes, dataType, destWaveH, &destWaveCreated))
  {
    throw IgorException(RetVal);
  }

  WaveClear(*destWaveH);
}

/// Need to call FinishDestWave after modifying data
template <typename T>
void MakeDestWave(T p, waveHndl *destWaveH, int dataType, const int numRows,
                  const int numCols, const std::vector<std::string> &rowNames,
                  const std::vector<std::string> &colNames)
{
  MakeDestWave(p, destWaveH, dataType, numRows, numCols);

  SetDimensionLabels(*destWaveH, ROWS, rowNames);

  SetDimensionLabels(*destWaveH, COLUMNS, colNames);
}

template <typename T>
void FinishDestWave(T p, waveHndl destWaveH)
{
  WaveHandleModified(destWaveH);

  // Set wave reference to refer to destination wave.
  if(p->destParamsSet[0] != 0)
  {
    if(int RetVal = SetOperationWaveRef(destWaveH, p->destParamsSet[0]))
    {
      throw IgorException(RetVal);
    }
  }
}

/// @brief Stores double data in a 64-bit wave
///
/// @tparam	T	type of data supplied
///
/// @param[in]	first	Pointer to first element of data to store
/// @param[in]	last	Pointer just past the last element to store
///
/// @param[out]	h	Handle to store wave in.
/// Caller must check that wave is of the correct type, and has the correct
/// size.
///
/// @param[in] dataAccessFn Function to convert T into double data.
/// @param[in] rowToWrite Index to row to put data in.
///
/// @return Igor error code.
template <typename T>
void storeDoubleData_waveRow(typename std::vector<T>::const_iterator first,
                             typename const std::vector<T>::const_iterator last,
                             waveHndl h,
                             std::function<double(const T &)> dataAccessFn,
                             size_t rowToWrite = 0)
{
  if(h == nullptr)
  {
    throw IgorException(NULL_WAVE_OP);
  }

  if(WaveType(h) != NT_FP64)
  {
    throw IgorException(WAVE_TYPE_MISMATCH);
  }

  // Read the dimension sizes
  int numDimensions = 0;
  CountInt dimensionSizes[MAX_DIMENSIONS + 1];
  MemClear(dimensionSizes, sizeof(dimensionSizes));
  if(int RetVal = MDGetWaveDimensions(h, &numDimensions, dimensionSizes))
  {
    throw IgorException(RetVal);
  }

  size_t numPtsToWrite = std::distance(first, last);
  size_t startIdx      = rowToWrite;
  size_t endIdx     = rowToWrite + dimensionSizes[ROWS] * (numPtsToWrite - 1);
  size_t numWavePts = (size_t) WavePoints(h);

  // Check the wave size
  if(numWavePts < endIdx)
  {
    // Wave is too small to hold all the data.
    // TODO: Is there a better error code we could return?
    throw IgorException(WAVE_LENGTH_MISMATCH);
  }

  double *dp = (double *) WaveData(h);

  // Write the data
  for(unsigned int currPoint = 0; currPoint < numPtsToWrite; currPoint++)
  {
    size_t currIdx = rowToWrite + currPoint * dimensionSizes[ROWS];
    dp[currIdx]    = dataAccessFn(first[currPoint]);
  }
}

template <typename T>
void writeChannelConfigWave(T p,
                            const std::vector<ITCChannelInfo> &ChannelSelection)
{

  // ResultWave: 64 - bit floating point Wave with at least 20 entries :
  const int numRows = 20;
  const int numCols = static_cast<int>(ChannelSelection.size());

  std::vector<std::string> RowNames{
      "",              // [0]  : Reserved
      "ChannelType",   // [1]  : ChannelType - integer value of channel type
                       // representation(see also Channel Type)
      "ChannelNumber", // [2]  : ChannelNumber - integer value of channel
                       // number(see also Channel Number)
      "",              // [3]  : Reserved
      "ErrorMode",  // [4]  : ErrorMode - integer representation of overflow and
                    // underrun flag settings
      "ErrorState", // [5]  : ErrorState - integer value representing any error
                    // condition
      "FIFOPointer", // [6]  : FIFOPointer - physical FIFO Pointer address
      "FIFONumberOfPoints",   // [7]  : FIFONumberOfPoints - size of data wave
      "",                     // [8]  : Reserved
      "",                     // [9]  : Reserved
      "",                     // [10] : Reserved
      "SamplingIntervalFlag", // [11] : SamplingIntervalFlag - integer
                              // representation of sampling rate sclaing factor
                              // and mode
      "SamplingRate", // [12] : SamplingRate - acquisition sampling interval
      "StartOffset",  // [13] : StartOffset - channel start offset relative to
                      // start of acquisition(currently not implemented)
      "Gain",   // [14] : Gain - channel software gain scaling factor(currently
                // not implemented)
      "Offset", // [15] : Offset - channel software offset factor(currently not
                // implemented)
      "Decimation", // [16] : Decimation - channel decimation factor
      "",           // [17] : Reserved;
      "",           // [18] : Reserved;
      "",           // [19] : Reserved;
  };
  std::vector<std::string> ColNames;

  // Destination wave handle
  waveHndl h = nullptr;

  const int dataType = NT_FP64;
  MakeDestWave(p, &h, dataType, numRows, numCols, RowNames, ColNames);

// Copy the data across
#define STOREDATAFN(Column, Field)                                             \
  storeDoubleData_waveRow<ITCChannelInfo>(                                     \
      ChannelSelection.begin(), ChannelSelection.end(), h,                     \
      [](const ITCChannelInfo &I) -> double { return (double) I.Field; },      \
      Column)

#define STOREDATAFN_PTR(Column, Field)                                         \
  storeDoubleData_waveRow<ITCChannelInfo>(                                     \
      ChannelSelection.begin(), ChannelSelection.end(), h,                     \
      [](const ITCChannelInfo &I) -> double {                                  \
        return (double) (UINT_PTR) I.Field;                                    \
      },                                                                       \
      Column)

  STOREDATAFN(0, ModeNumberOfPoints);
  STOREDATAFN(1, ChannelType);
  STOREDATAFN(2, ChannelNumber);
  STOREDATAFN(3, ChannelMode);

  STOREDATAFN(4, ErrorMode);
  STOREDATAFN(5, ErrorState);
  STOREDATAFN_PTR(6, FIFOPointer);
  STOREDATAFN(7, FIFONumberOfPoints);

  STOREDATAFN(8, ModeOfOperation);
  STOREDATAFN(9, SizeOfModeParameters);
  STOREDATAFN_PTR(10, ModeParameters);
  STOREDATAFN(11, SamplingIntervalFlag);

  STOREDATAFN(12, SamplingRate);
  STOREDATAFN(13, StartOffset);
  STOREDATAFN(14, Gain);
  STOREDATAFN(15, Offset);

  STOREDATAFN(16, ExternalDecimation);
  STOREDATAFN(17, HardwareUnderrunValue);
  STOREDATAFN(18, Reserved2);
  STOREDATAFN(19, Reserved3);

#undef STOREDATAFN
#undef STOREDATAFN_PTR

  FinishDestWave(p, h);
}

// @brief Check a wave is a 2D wave, of the correct data type, with the correct
// minimum number of rows.
//
// @return 2-element vector containing the dimension sizes.
std::vector<size_t> checkWave_2D(waveHndl waveH, int dataType,
                                 size_t minNumRows);

size_t getOffset_2D(size_t Row, size_t Col,
                    const std::vector<size_t> &dimSizes);

template <typename outT, typename T>
outT GetChannelNumberFromParameters(T *p)
{
  if(p->channelNumberEncountered)
  {
    // Parameter: p->channelNumber
    return lockToIntegerRange<outT>(p->channelNumber);
  }

  // No channelNumber encountered
  throw IgorException(OPAND_MISMATCH);
}
