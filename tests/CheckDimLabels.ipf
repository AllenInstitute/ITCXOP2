#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

/// @brief Check dimension labels which are set for a wave
///
/// @param dimension -- Dimension to check
/// @param testWave -- Wave to check dimension labels on
/// @param labelsWave -- Wave containing desired labels
/// @param skipWave -- Wave containing zero (do not skip) or 1 (skip and do not check)
Function CheckDimLabels(dimension, testWave, labelsWave, skipWave)
	Variable dimension
	Wave testWave
	Wave /T labelsWave
	Wave skipWave

	Variable k
	For( k = 0; k < DimSize(testWave, dimension); k += 1)

		// Check that we've not been asked to skip this label
		// In order to skip:
		// -> skipWave must be non-null
		// -> k must refer to a valid index in skipWave
		// -> "Skip" must be set >0 to skip
		If( WaveExists(skipWave) && (k < numpnts(skipWave)) && skipWave[k] > 0 )
			Continue
		EndIf


		// Check that we have a label to test
		// -> If k is not a valid index in labelsWave, and we've not been told to skip, fail
		If( !( k < numpnts(labelsWave)))
			FAIL()
		EndIf

		// Get the dimension label
		String currDimLabel = GetDimLabel(testWave, dimension, k)
		String desiredDimLabel = labelsWave[k]

		CHECK_EQUAL_STR(currDimLabel, desiredDimLabel, case_sensitive = 1)

		If( !StringMatch(currDimLabel, desiredDimLabel) )
			printf "Dimension label mismatch.  Expected %s, got %s\n", desiredDimLabel, currDimLabel
		EndIf

	EndFor

End
