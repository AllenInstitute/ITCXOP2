#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=Test_GetVersions2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/byte-physics/igor-unit-testing-framework
#include "unit-testing"

// This also tests the logic for not allowing /FREE in Macros but allowing it in functions

static Function Works()

	ITCGetVersions2/FREE versions
	CHECK_WAVE(versions, TEXT_WAVE | FREE_WAVE)
End

threadsafe static Function/WAVE WorksInThreadsafeFunctionImpl()

	ITCGetVersions2/FREE versions

	return versions
End

static Function WorksInThreadsafeFunction()

	WAVE versions = WorksInThreadsafeFunctionImpl()
	CHECK_WAVE(versions, TEXT_WAVE | FREE_WAVE)
End

Macro ComplainsInMacroImpl()

	ITCGetVersions2/FREE/Z=1 versions
EndMacro

static Function ComplainsInMacro()

	Execute/Z/Q "variable/G V_ITCError = NaN, V_ITCXOPError = NaN"
	Execute/Z/Q "ComplainsInMacroImpl()"
	NVAR/Z V_ITCError, V_ITCXOPError
	CHECK(NVAR_Exists(V_ITCError))
	CHECK(NVAR_Exists(V_ITCXOPError))
	CHECK_EQUAL_VAR(V_ITCError, 0)
	CHECK_EQUAL_VAR(V_ITCXOPError, 6109)
	WAVE/Z versions
	CHECK_WAVE(versions, NULL_WAVE)
End
