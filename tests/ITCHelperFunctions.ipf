#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// @brief Make a config wave for ITCConfigAllChannels2
///
/// Wave is of the correct type, correctly sized and has dimensions labelled.
///
/// @param numChannels - number of channels to configure
///
/// @return reference to config wave
///
/// Assign to a global wave (if required) using, for example,
///
/// 	Duplicate makeConfigAllWave(1), globalConfigWave
///
threadsafe Function /WAVE makeConfigAllWave(numChannels)
	Variable numChannels

	Make /D /FREE /N=(4, numChannels) confWave

	Variable dimension = 0
	Make /FREE /T labelWave = {"ChannelType","ChannelNumber","SamplingInterval","Decimate"}

	setDimLabels(confWave, labelWave, dimension)

	return confWave
End

/// @brief Make a FIFO wave for ITCConfigAllChannels2
///
/// Wave is of the correct type, and is correctly sized
///
/// @param numPoints -- number of points in fifoWave
/// @param numChannels -- number of channels of data
///
/// @return reference to FIFO wave
///
/// The result should be assigned to a global wave before using with ITCConfigAllChannels2,
/// otherwise a dangling reference results.
///
/// 	Duplicate makeFIFOWave(100, 1), globalFIFOWave
///
threadsafe Function /WAVE makeFIFOWave(numPoints, numChannels)
	Variable numPoints, numChannels

	Make /W /FREE /N=(numPoints, numChannels) confWave

	return confWave
End

/// @brief Make a channel selection wave
///
/// Wave is of the correct type, correctly sized and has dimensions labelled.
///
/// @param numChannels - number of channels to include in selection wave
///
/// @return reference to selection wave
threadsafe Function /WAVE makeChannelSelectionWave(numChannels)
	Variable numChannels

	Make /FREE /N=(2, numChannels) selectionWave

	Variable dimension = 0
	Make /FREE /T labelWave = {"ChannelType","ChannelNumber"}

	setDimLabels(selectionWave, labelWave, dimension)

	return selectionWave
End


/// @brief Make a FIFO position wave, for use with ITCUpdateFIFOPositionAll2
///
/// Wave is of the correct type, correctly sized and has dimensions labelled.
///
/// @param numChannels - number of channels to include in selection wave
///
/// @return reference to FIFO position wave
threadsafe Function /WAVE makeFIFOPositionWave(numChannels)
	Variable numChannels

	Make /FREE /D /N=(4, numChannels) fifoPositionWave

	Variable dimension = 0
	Make /FREE /T labelWave = {"ChannelType","ChannelNumber", "Value", ""}

	setDimLabels(fifoPositionWave, labelWave, dimension)

	return fifoPositionWave
End


/// @brief Set the dimension labels on a wave
///
/// @param dataWave -- wave to set labels on
/// @param labelWave -- text wave containing labels to set
/// @param dimension -- dimension along which to set the labels
threadsafe Static Function setDimLabels(dataWave, labelWave, dimension)
	Wave dataWave
	Wave /T labelWave
	Variable dimension

	Variable k
	For (k=0; k < numpnts(labelWave); k++)
		SetDimLabel dimension, k, $labelWave[k], dataWave
	EndFor
End
