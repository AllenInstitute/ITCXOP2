#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=GenerateDestTest

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// ASSUMPTIONS
// - The destination wave is the final parameter
// - The command acceps /O and /FREE
//
// The destination wave is automatically appended
// The parameter string must be able to be preceded by /O and /FREE
// Any commas that need to be present before the destination wave must
// be included in the parameterString
//
// Example:
//
// Full command:
// ITCReadTimer2 /FREE defaultMode, outWave
//
// commandString: "ITCReadTimer2"
// parameterString "defaultMode,"
//

Function GenITCFIFOAvailableAll2_DT()
	PutScrapText GenerateDestTest(" ITCFIFOAvailableAll2", "*,")
End


Function GenITCGetAllChannelsConfig2_DT()
	PutScrapText GenerateDestTest(" ITCGetAllChannelsConfig2", "*,")
End


Function GenITCGetChannelConfig2_DT()
	PutScrapText GenerateDestTest("ITCGetChannelConfig2", "/CHN=(defaultCHN) defaultChannel,")
End


Function GenITCGetState2_DT()
	PutScrapText GenerateDestTest("ITCGetState2", "/ALL ")
End


Function GenITCReadTimer2_DT()
	PutScrapText GenerateDestTest("ITCReadTimer2", "defaultMode,")
End

Function /S GenerateDestTest(commandString, parameterString)
	String commandString, parameterString

	String destTestString = GetDestTestTemplate()

	destTestString = ReplaceString("%%COMMAND%%", destTestString, commandString)

	destTestString = ReplaceString("%%PARAMETERS%%", destTestString, parameterString)

	return destTestString
End

Static Function /S GetDestTestTemplate()

String S = ""

	S += "Static Function DestTest()\n"
	S += "	variable error\n"
	S += "	String outWaveName = \"outWave_string\"\n"
	S += "	// String freeOutWaveName = \"freeOutWave_string\"\n"
	S += "	Variable NAME_WAV_CONFLICT = 27\n"
	S += "\n"
	S += "//------------------------------------------ \n"
	S += "// Create waves\n"
	S += "\n"
	S += "// Normal wave\n"
	S += "	%%COMMAND%% %%PARAMETERS%% outWave\n"
	S += "	\n"
	S += "// Wave name\n"
	S += "	%%COMMAND%% %%PARAMETERS%% $outWaveName\n"
	S += "	\n"
	S += "//------------------------------------------ 	\n"
	S += "// Overwrite -- fail if /O flag not specified\n"
	S += "	try\n"
	S += "		%%COMMAND%% %%PARAMETERS%% outWave; AbortOnRTE\n"
	S += "		FAIL()\n"
	S += "	catch\n"
	S += "		error = GetRTError(1)\n"
	S += "		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)\n"
	S += "		CHECK_EQUAL_VAR(V_ITCError, 0)\n"
	S += "	endtry\n"
	S += "	\n"
	S += "	try\n"
	S += "		%%COMMAND%% %%PARAMETERS%% $outWaveName; AbortOnRTE\n"
	S += "		FAIL()\n"
	S += "	catch\n"
	S += "		error = GetRTError(1)\n"
	S += "		CHECK_EQUAL_VAR(V_ITCXOPError, NAME_WAV_CONFLICT)\n"
	S += "		CHECK_EQUAL_VAR(V_ITCError, 0)\n"
	S += "	endtry\n"
	S += "	\n"
	S += "//------------------------------------------ \n"
	S += "// Overwrite -- OK if /O flag specified\n"
	S += "	%%COMMAND%% /O %%PARAMETERS%% outWave\n"
	S += "	%%COMMAND%% /O %%PARAMETERS%% $outWaveName\n"
	S += "\n"
	S += "//------------------------------------------ \n"
	S += "// Free wave\n"
	S += "	%%COMMAND%% /FREE %%PARAMETERS%% freeOutWave\n"
	S += "	CHECK_EQUAL_VAR(WaveType(freeOutWave, 2), 2)\n"
	S += "	\n"
	S += "	// So, it seems combining /FREE with string-to-wave reference $ doesn't work properly.\n"
	S += "	// Trying to call\n"
	S += "	//    Make /FREE $waveName\n"
	S += "	// gives an error before compilation.\n"
	S += "	//\n"
	S += "	// Not sure why we don't get an error before compilation here...\n"
	S += "	\n"
	S += "	// %%COMMAND%% /FREE %%PARAMETERS%% $freeOutWaveName\n"
	S += "	// CHECK_EQUAL_VAR(WaveType(freeOutWave2, 2), 2)\n"
	S += "\n"
	S += "//------------------------------------------\n"
	S += "// Free wave overwrites don't fail\n"
	S += "// (the wave reference is updated to the new wave)\n"
	S += "\n"
	S += "	%%COMMAND%% /FREE %%PARAMETERS%% freeOutWave;\n"
	S += "	CHECK_EQUAL_VAR(V_ITCXOPError, 0)\n"
	S += "	CHECK_EQUAL_VAR(V_ITCError, 0)\n"
	S += "	\n"
	S += "	// %%COMMAND%% /FREE %%PARAMETERS%% $freeOutWaveName\n"
	S += "	// CHECK_EQUAL_VAR(V_ITCXOPError, 0)\n"
	S += "	// CHECK_EQUAL_VAR(V_ITCError, 0)\n"
	S += "	\n"
	S += "//------------------------------------------ \n"
	S += "// Free wave overwrite should also work if the /O flag specified\n"
	S += "	%%COMMAND%% /O /FREE %%PARAMETERS%% freeOutWave\n"
	S += "	// %%COMMAND%% /O /FREE %%PARAMETERS%% $freeOutWaveName\n"
	S += "End\n"

return S
End