#pragma TextEncoding = "UTF-8"		// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		// Use modern global access method and strict wave access.
#pragma ModuleName=Test_ITCSetGlobals2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/byte-physics/igor-unit-testing-framework
#include "unit-testing"

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static Constant DEVICE_ID     = 0

// Run with: RunTest("Test_ITCSetGlobals2.ipf")

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	string path

	// Close any devices left open
	ITCCloseAll2

	ApplySafeGlobalDefaults()

	path = GetLogFilePath_IGNORE()

	DeleteFile/Z=1 path
	CHECK(V_Flag == 0 || V_Flag == -43)
End

static Function/S GetLogFilePath_IGNORE()

	string path

	path = SpecialDirPath("Igor Preferences", 0, 0, 1) + "Packages:"
	NewPath/Q/C/O myPath, path
	path += "ITCXOP2:"
	NewPath/Q/C/O myPath, path
	path += "Log.jsonl"

	return path
End

static Function/S ReadFile(string path)

	variable refNum
	string contents

	Open/R/Z=1 refNum as path

	CHECK_EQUAL_VAR(V_Flag, 0)

	FStatus refNum

	contents = PadString("", V_logEOF, 0x0)
	FBinRead refnum, contents
	Close refnum

	return contents
End

static Function noFlag()
	ITCSetGlobals2
	PASS()
End

static Function zFlag()

	ITCSetGlobals2/Z
	ITCSetGlobals2/Z=0
	ITCSetGlobals2/Z=1
	PASS()
End

static Function dFlag()

	ITCSetGlobals2/D
	ITCSetGlobals2/D=0
	ITCSetGlobals2/D=1
	PASS()
End

static Function/WAVE InvalidLoggingTemplates()

	Make/FREE/T wv = {"}", "[]", "{\"json\": null}", "{\"str\": null}", "{\"ts\": null}"}
	
	return wv
End

// UTF_TD_GENERATOR Test_ITCSetGlobals2#InvalidLoggingTemplates
static Function LTSFlagInvalidTemplate([string str])

	variable err

	try
		ITCSetGlobals2/LTS=str; AbortONRTE
		FAIL()
	catch
		err = GetRTError(1)
		PASS()
	endtry
End

static Function LTSFlagWorks()
	string path, actual, expected

	path = GetLogFilePath_IGNORE()
	GetFileFolderInfo/Q/Z=1 path
	CHECK_NEQ_VAR(V_flag, 0)

	ITCSetGlobals2/LTS="{\"blahh\" : \"blubb\"}"

	GetFileFolderInfo/Q/Z=1 path
	CHECK_EQUAL_VAR(V_flag, 0)

	actual   = ReadFile(path)
	expected = "{}\n"
	CHECK_EQUAL_STR(actual, expected)

	// nothing gets added when debugging if off
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID

	actual   = ReadFile(path)
	expected = "{}\n"
	CHECK_EQUAL_STR(actual, expected)

	ITCCloseAll2

	// but if we turn it on
	ITCsetGlobals2/D=1
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID

	// we have debug output
	actual = ReadFile(path)
	CHECK_GT_VAR(strsearch(actual, "{\"blahh\":\"blubb\"", 0), 0)
End
