#pragma rtGlobals=3		// Use modern global access method and strict wave access.

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

Function CheckParamsStruct(FunctionName, File, Template)
	String FunctionName, File, Template

	String StartString = "// Runtime param structure for " + FunctionName + " operation."
	String EndString = "#pragma pack()	// Reset structure alignment to default."

	Variable FileStartPos = strsearch(File, StartString, 0)
	Variable FileEndPos = strsearch(File, EndString, 0)

	Variable TemplateStartPos = strsearch(Template, StartString, 0)
	Variable TemplateEndPos = strsearch(Template, EndString, 0)

	String FileStructString = File[FileStartPos,FileEndPos]
	String TemplateStructString = Template[TemplateStartPos, TemplateEndPos]

	// Fix up line endings
	FileStructString = ReplaceString("\r\n", FileStructString, "\r")

	If( !StringMatch(FileStructString, TemplateStructString) )
		Abort "Problem with Params Struct in " + functionName
	EndIf
End

Function CheckRegisterFunction(functionName, File, Template, RetValString, SRetValString)
	String functionName, File, Template, RetValString, SRetValString

	String StartString = "Register" + functionName + "(void)"
	String EndString = "}"

	Variable FileStartPos = strsearch(File, StartString, 0)
	Variable FileEndPos = strsearch(File, EndString, strlen(File), 1)

	Variable TemplateStartPos = strsearch(Template, StartString, 0)
	Variable TemplateEndPos = strsearch(Template, EndString, strlen(Template), 1)

	String FileRegisterString = File[FileStartPos,FileEndPos]
	String TemplateRegisterString = Template[TemplateStartPos, TemplateEndPos]

	// Fix up line endings
	FileRegisterString = ReplaceString("\r\n", FileRegisterString, "\r")

	// Fix up the runtimeNumVarList
	// This will contain: EXCEPTION_VARS
	// and, optionally, RETURN_VARIABLE, if a numeric return variable is set.
	If( StrLen(RetValString) == 0 )
		TemplateRegisterString = ReplaceString("runtimeNumVarList = \"\";", TemplateRegisterString, "runtimeNumVarList = EXCEPTION_VARS;")
	Else
		TemplateRegisterString = ReplaceString("runtimeNumVarList = \"\";", TemplateRegisterString, "runtimeNumVarList = EXCEPTION_VARS \";\" RETURN_VARIABLE;")
	EndIf

	// Fix up the runtimeStrVarList
	// This will contain RETURN_VARIABLE, if a string return variable is set.
	If( StrLen(SRetValString) > 0 )
		TemplateRegisterString = ReplaceString("runtimeStrVarList = \"\";", TemplateRegisterString, "runtimeStrVarList = RETURN_VARIABLE;")
	EndIf

	If( !StringMatch(FileRegisterString, TemplateRegisterString) )
		Abort "Problem with RegisterFunction in " + functionName
	EndIf
End

Function /S getFunctionNameFromTemplate(TemplateString)
	String TemplateString

	// Work out what the function name is.
	// Search for "/" or " " or end of string
	Variable slashPos = strsearch(TemplateString, "/", 0)
	Variable spacePos = strsearch(TemplateString, " ", 0)

	String functionName

	If( slashPos == -1 && spacePos == -1 )
		functionName = TrimString(TemplateString)
	ElseIf( slashPos == -1 )
		functionName = TrimString(TemplateString[0, spacePos-1])
	ElseIf( spacePos == -1 )
		functionName = TrimString(TemplateString[0, slashPos-1])
	Else
		functionName = TrimString(TemplateString[0, min(spacePos, slashPos)-1])
	EndIf

	return functionName
End

Function CheckReturnValName(functionName, cppFile, RetValString, SRetValString)
	String functionName, cppFile, RetValString, SRetValString

	If( StrLen(RetValString) > 0 )
		If(StringMatch(cppFile, "#define RETURN_VARIABLE \"" + RetValString + "\"") == -1)
			Abort "Problem with return variable name in " + functionName
		EndIf
	EndIf


	If( StrLen(SRetValString) > 0 )
		If(StringMatch(cppFile, "#define RETURN_VARIABLE \"" + SRetValString + "\"") == -1)
			Abort "Problem with return variable name in " + functionName
		EndIf
	EndIf
End

Function VerifyTemplate(TemplateString, RetValString, SRetValString)
	String TemplateString, RetValString, SRetValString

	String functionName = getFunctionNameFromTemplate(TemplateString)

	// Open the .cpp file
	// Get the path to the file
	String cppPath = ParseFilePath(1, FunctionPath("VerifyTemplate"), ":", 1, 1) + "ITC:itcXOP2:"
	cppPath = ParseFilePath(5, cppPath, "*", 0, 0)
	String cppFileName = cppPath + functionName + ".cpp"
	Variable fid
	Open /R fid as cppFileName

	String cppFile
	FReadLine /T="" fid, cppFile

	// Check that the Operation Template is found correctly
	Variable OperationTemplate = strsearch(cppFile, "// Operation template: " +  TemplateString, 0)
	If(OperationTemplate == -1)
		Abort "Operation Template not found"
	EndIf

	ParseOperationTemplate/C=6/S=1/T/TS TemplateString
	If (V_Flag != 0)
		Abort "Error with template"
	EndIf

	String cppTemplate = GetScrapText()

	CheckParamsStruct(functionName, cppFile, cppTemplate)

	CheckRegisterFunction(functionName, cppFile, cppTemplate, RetValString, SRetValString)

	CheckReturnValName(functionName, cppFile, RetValString, SRetValString)
End

Function GenerateITCOperationCode()
	// All operations set V_ITCError and V_ITCXOPError
	//
	// The C++ code always has to check that one of
	// /CHN=number:channelTypeNumeric/CHS=string:channelTypeString
	// is given. Same for deviceTypeNumeric and deviceTupeString
	//
	// All waves which are written to but not read from by the operations
	// are resized/retyped as required. These waves should then all have
	// dimension labels too.

	String ITCCloseAll2Template = "ITCCloseAll2/Z[=number:displayErrors]"
	ParseOperationTemplate/C=6/S=1/T/TS ITCCloseAll2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCCloseAll2Template, "", "")

	String ITCCloseDevice2Template = "ITCCloseDevice2/Z[=number:displayErrors]/DEV=number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCCloseDevice2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCCloseDevice2Template, "", "")

	String ITCConfigChannelReset2Template = "ITCConfigChannelReset2/Z[=number:displayErrors]/DEV=number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCConfigChannelReset2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCConfigChannelReset2Template, "", "")

	String ITCConfigChannelUpload2Template = "ITCConfigChannelUpload2/Z[=number:displayErrors]/DEV=number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCConfigChannelUpload2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCConfigChannelUpload2Template, "", "")

	String ITCConfigAllChannels2Template = "ITCConfigAllChannels2/Z[=number:displayErrors]/DEV=number:deviceID wave:config, wave:data"
	ParseOperationTemplate/C=6/S=1/T/TS ITCConfigAllChannels2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCConfigAllChannels2Template, "", "")

	String ITCConfigChannel2Template  = "ITCConfigChannel2/Z[=number:displayErrors]/S=number:scale/M=number:mode/A[=number:adjustRate]/O=number:overflow/U=number:underrun"
	ITCConfigChannel2Template += "/D=number:decimate /DEV=number:deviceID"
	ITCConfigChannel2Template += "/CHN=number:channelTypeNumeric/CHS=string:channelTypeString number:channelNumber, number:samplingInterval, wave:data"
	ParseOperationTemplate/C=6/S=1/T/TS ITCConfigChannel2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCConfigChannel2Template, "", "")

	// return position in V_Value
	String ITCFIFOAvailable2Template  = "ITCFIFOAvailable2/Z[=number:displayErrors] /DEV=number:deviceID"
	ITCFIFOAvailable2Template += "/CHN=number:channelTypeNumeric/CHS=string:channelTypeString number:channelNumber"
	String ITCFIFOAvailable2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCFIFOAvailable2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCFIFOAvailable2Template, ITCFIFOAvailable2RetVals, "")

	// channel types and numbers are queried from channelSelection
	String	ITCFIFOAvailableAll2Template
	ITCFIFOAvailableAll2Template = "ITCFIFOAvailableAll2/Z[=number:displayErrors]"
	ITCFIFOAvailableAll2Template += "/O/FREE/DEV=number:deviceID wave:{channelSelection,real}, DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCFIFOAvailableAll2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCFIFOAvailableAll2Template, "", "")

	// channel types and numbers are queried from channelSelection
	String ITCGetAllChannelsConfig2Tmpl
	ITCGetAllChannelsConfig2Tmpl = "ITCGetAllChannelsConfig2/Z[=number:displayErrors]"
	ITCGetAllChannelsConfig2Tmpl += "/O/FREE/DEV=number:deviceID wave:{channelSelection,real}, DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetAllChannelsConfig2Tmpl
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetAllChannelsConfig2Tmpl, "", "")

	String ITCGetChannelConfig2Template = "ITCGetChannelConfig2/Z[=number:displayErrors] /DEV=number:deviceID"
	ITCGetChannelConfig2Template += "/CHN=number:channelTypeNumeric/CHS=string:channelTypeString/O/FREE number:channelNumber, DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetChannelConfig2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetChannelConfig2Template, "", "")

	// returns currently active device in V_Value
	String ITCGetCurrentDevice2Template = "ITCGetCurrentDevice2/Z[=number:displayErrors]"
	String ITCGetCurrentDevice2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetCurrentDevice2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetCurrentDevice2Template, ITCGetCurrentDevice2RetVals, "")

	String ITCGetDeviceInfo2Template = "ITCGetDeviceInfo2/Z[=number:displayErrors]/DEV=number:deviceID /O /FREE DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetDeviceInfo2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetDeviceInfo2Template, "", "")

	// Return number of devices in V_Value
	String ITCGetDevices2Template = "ITCGetDevices2/Z[=number:displayErrors]"
	ITCGetDevices2Template += "/DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString"
	String ITCGetDevices2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetDevices2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetDevices2Template, ITCGetDevices2RetVals, "")

	// returns S_errorMessage
	String ITCGetErrorString2Template = "ITCGetErrorString2/Z[=number:displayErrors]/DEV[=number:deviceID] /X number:errorCode"
	String ITCGetErrorString2SRetVals = "S_errorMessage"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetErrorString2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetErrorString2Template, "", ITCGetErrorString2SRetVals)

	// ITCGetFIFOInformation and ITCGetFIFOInformationAll: documentation does not match code
	// @todo need to think about a proper interface as we currently don't use these functions

// FUNCTION NOT IMPLEMENTED
//
//	// returns S_errorMessage
//	String ITCGetLastError2Template = "ITCGetLastError2/Z[=number:displayErrors] number:errorCode"
//	String ITCGetLastError2SRetVals = "S_errorMessage"
//	ParseOperationTemplate/C=6/S=1/T/TS ITCGetLastError2Template
//	AbortOnValue V_flag, 0

	// created wave will be of type 32bit unsigned integer
	String ITCGetSerialNumber2Template = "ITCGetSerialNumber2/Z[=number:displayErrors]/DEV=number:deviceID/O/FREE DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetSerialNumber2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetSerialNumber2Template, "", "")

	// created wave will be of type 32bit signed integer
	String ITCGetState2Template = "ITCGetState2/Z[=number:displayErrors]/DEV=number:deviceID"
	ITCGetState2Template += "/R/OF/C/E/ALL /O/FREE DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetState2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetState2Template, "", "")

	String ITCGetVersion2Template = "ITCGetVersions2/Z[=number:displayErrors]/DEV=number:deviceID /O /FREE DataFolderAndName:{dest,text}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCGetVersion2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCGetVersion2Template, "", "")

	String ITCInitialize2Template = "ITCInitialize2/Z[=number:displayErrors] /DEV=number:deviceID"
	ITCInitialize2Template += "/M=number:mode/U=string:file/F=number:fFunc/D=number:dFunc/H=number:hFunc/R=number:rFunc"
	ParseOperationTemplate/C=6/S=1/T/TS ITCInitialize2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCInitialize2Template, "", "")

	// return deviceID in V_value
	String ITCOpenDevice2Template  = "ITCOpenDevice2/Z[=number:displayErrors]"
	ITCOpenDevice2Template += "/DTN=number:deviceTypeNumeric/DTS=string:deviceTypeString number:deviceNumber"
	String ITCOpenDevice2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCOpenDevice2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCOpenDevice2Template, ITCOpenDevice2RetVals, "")

	// return read value in V_Value
	String ITCReadADC2Template = "ITCReadADC2/Z[=number:displayErrors]/DEV=number:deviceID/V[=number:voltageOn]/C[=number:calibrateOn] number:channelNumber"
	String ITCReadADC2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCReadADC2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCReadADC2Template, ITCReadADC2RetVals, "")

	// return read voltages/DACUnits in V_Value
	String ITCReadDigital2Template = "ITCReadDigital2/Z[=number:displayErrors]/DEV=number:deviceID number:channelNumber"
	String ITCReadDigitial2RetVals = "V_Value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCReadDigital2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCReadDigital2Template, ITCReadDigitial2RetVals, "")

	String ITCReadTimer2Template = "ITCReadTimer2/Z[=number:displayErrors]/DEV=number:deviceID/O/FREE number:mode, DataFolderAndName:{dest,real}"
	ParseOperationTemplate/C=6/S=1/T/TS ITCReadTimer2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCReadTimer2Template, "", "")

	String ITCSelectDevice2Template = "ITCSelectDevice2/Z[=number:displayErrors] number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCSelectDevice2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCSelectDevice2Template, "", "")

	String ITCSetDAC2Template = "ITCSetDAC2/Z[=number:displayErrors]/DEV=number:deviceID/V[=number:voltageOn]/C[=number:calibrateOn] number:channelNumber, number:voltageOrDACUnits"
	ParseOperationTemplate/C=6/S=1/T/TS ITCSetDAC2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCSetDAC2Template, "", "")

	// /Z ignores errors
	// /D will cause each operation to output debug information
	String ITCSetGlobals2Template = "ITCSetGlobals2/Z[=number:displayErrors]/D[=number:debugOutput]"
	ParseOperationTemplate/C=6/S=1/T/TS ITCSetGlobals2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCSetGlobals2Template, "", "")

	String ITCSetModes2Template  = "ITCSetModes2/Z[=number:displayErrors]/DEV=number:deviceID"
	ITCSetModes2Template += "/I=number:DigitalInputMode/E=number:ExternalTriggerMode/T=number:ExternalTrigger/C[=number:EnableExternalClock]/D=number:DACShiftValue"
	ITCSetModes2Template += "/R=number:InputRange/P=number:TriggerOutPosition/O[=number:OutputEnable]/F[=number:ResetFIFOFlag]/L=number:ControlLight/S=number:SamplingInterval"
	ParseOperationTemplate/C=6/S=1/T/TS ITCSetModes2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCSetModes2Template, "", "")

	String ITCSetState2Template = "ITCSetState2/Z[=number:displayErrors]/S=number:switchCommand/DEV=number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCSetState2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCSetState2Template, "", "")

// FUNCTION NOT IMPLEMENTED
//
//	String ITCShortAcquisition2Template = "ITCShortAcquisition2/Z[=number:displayErrors] wave:config, wave:data"
//	ParseOperationTemplate/C=6/S=1/T/TS ITCShortAcquisition2Template
//	AbortOnValue V_flag, 0

	String ITCStartAcq2Template = "ITCStartAcq2/Z[=number:displayErrors]/DEV=number:deviceID/R[=number:resetFIFOs]/F[=number:fastPointerUpdate]/OE=number:outputEnable/EXT=number:externalTrigger"
	ITCStartAcq2Template += "/STPO=number:stopOnOverflow/STPU=number:stopOnUnderrun/STRT=number:startTime/STOP=number:stopTime"
	ParseOperationTemplate/C=6/S=1/T/TS ITCStartAcq2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCStartAcq2Template, "", "")

	String ITCStopACq2Template = "ITCStopAcq2/Z[=number:displayErrors]/DEV=number:deviceID"
	ParseOperationTemplate/C=6/S=1/T/TS ITCStopACq2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCStopACq2Template, "", "")

	String ITCUpdateFIFOPosition2Template  = "ITCUpdateFIFOPosition2/Z[=number:displayErrors]/DEV=number:deviceID"
	ITCUpdateFIFOPosition2Template += "/L[=number:lastFIFO] /R[=number:resetFIFO] /CHN=number:channelTypeNumeric/CHS=string:channelTypeString number:channelNumber, number:position"
	ParseOperationTemplate/C=6/S=1/T/TS ITCUpdateFIFOPosition2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCUpdateFIFOPosition2Template, "", "")

	String ITCUpdateFIFOPositionAll2Tmpl = "ITCUpdateFIFOPositionAll2/Z[=number:displayErrors]/DEV=number:deviceID wave:fifoPos"
	ParseOperationTemplate/C=6/S=1/T/TS ITCUpdateFIFOPositionAll2Tmpl
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCUpdateFIFOPositionAll2Tmpl, "", "")

	String ITCWriteDigital2Template = "ITCWriteDigital2/Z[=number:displayErrors] /DEV=number:deviceID number:channelNumber, number:value"
	ParseOperationTemplate/C=6/S=1/T/TS ITCWriteDigital2Template
	AbortOnValue V_flag, 0
	VerifyTemplate(ITCWriteDigital2Template, "", "")
End
