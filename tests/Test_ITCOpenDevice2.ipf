#pragma TextEncoding = "UTF-8"	// For details execute DisplayHelpTopic "The TextEncoding Pragma"
#pragma rtGlobals=3		          // Use modern global access method and strict wave access.
#pragma ModuleName=ITCOpenDevice2

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// available from https://github.com/t-b/igor-unit-testing-framework
#include "unit-testing"

// Run with: RunTest("Test_ITCOpenDevice2.ipf")
// and disable debugger

// assumes a connected ITC18 USB
Static Constant DEVICE_NUM    = 5
Static StrConstant DEVICE_STR = "ITC18USB"
Static Constant DEVICE_ID     =  0

Static Function TEST_CASE_BEGIN_OVERRIDE(name)
	string name

	ITCCloseAll2

	TEST_CASE_BEGIN(name)
End

Function init_device_num()
	ITCOpenDevice2/DTN=(DEVICE_NUM) DEVICE_ID
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)
	CHECK(V_Value >= 0)
End

Function init_device_str()
	ITCOpenDevice2/DTS="ITC18USB" 0
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 0)
	CHECK(V_Value >= 0)
End

Function invalid_devType_num()

	variable error

	try
		ITCOpenDevice2/DTN=(4711) DEVICE_ID; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 10007)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	CHECK_EQUAL_VAR(V_Value, -1)
End

Function invalid_devType_str()

	variable error

	try
		ITCOpenDevice2/DTS="abcd" DEVICE_ID; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 10008)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	CHECK_EQUAL_VAR(V_Value, -1)
End

Function invalid_devType_num_1()

	variable error

	try
		ITCOpenDevice2/DTN=(DEVICE_NUM) NaN; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 863)
	CHECK_EQUAL_VAR(V_ITCError, 0)
	CHECK_EQUAL_VAR(V_Value, -1)
End

Function invalid_devType_num_2()

	variable error

	try
		ITCOpenDevice2/DTN=(DEVICE_NUM) inf; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 863)
	CHECK_EQUAL_VAR(V_ITCError, 0)
	CHECK_EQUAL_VAR(V_Value, -1)
End

Function invalid_devType_num_3()

	variable error

	try
		ITCOpenDevice2/DTN=(DEVICE_NUM) -1; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 823)
	CHECK_EQUAL_VAR(V_ITCError, 0)

	CHECK_EQUAL_VAR(V_Value, -1)
End

Function invalid_devType_num_4()

	variable error

	try
		ITCOpenDevice2/DTN=(DEVICE_NUM) 1024; AbortOnRTE
		FAIL()
	catch
		error = GetRTError(1)
	endtry
	CHECK_EQUAL_VAR(V_ITCXOPError, 0)
	CHECK_EQUAL_VAR(V_ITCError, 4026535937)

	CHECK_EQUAL_VAR(V_Value, -1)
End
