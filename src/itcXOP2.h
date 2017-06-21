#pragma once

// This file is part of the `ITCXOP2` project and licensed under BSD-3-Clause.

// Required Igor versions
#ifdef _WIN64
// Require Igor 7.0 for 64-bit versions
#define MIN_IGOR_VERSION 700
#else
// Require Igor 6.3 for 32-bit versions
#define MIN_IGOR_VERSION 630
#endif

// clang-format off

// Custom error codes
#define OLD_IGOR                               1  + FIRST_XOP_ERR
#define UNHANDLED_CPP_EXCEPTION                2  + FIRST_XOP_ERR

// Errors related to DeviceIDs
#define SLOT_LOCKED_TO_OTHER_THREAD            3  + FIRST_XOP_ERR
#define SLOT_EMPTY                             4  + FIRST_XOP_ERR
#define COULDNT_FIND_EMPTY_SLOT                5  + FIRST_XOP_ERR

// ITC DLL errors
#define ITC_DLL_ERROR                          6  + FIRST_XOP_ERR

// /DTN and /DTS
#define INVALID_DEVICETYPE_NUMERIC             7  + FIRST_XOP_ERR
#define INVALID_DEVICETYPE_STRING              8  + FIRST_XOP_ERR
#define DTN_DTS_DISAGREE                       9  + FIRST_XOP_ERR

// /CHN and /CHS
#define INVALID_CHANNELTYPE_NUMERIC            10 + FIRST_XOP_ERR
#define INVALID_CHANNELTYPE_STRING             11 + FIRST_XOP_ERR
#define CHN_CHS_DISAGREE                       12 + FIRST_XOP_ERR
#define MUST_SPECIFY_CHN_OR_CHS                13 + FIRST_XOP_ERR

// ITCConfigChannel2 flags
#define ITCCONFIGCHANNEL2_BAD_S                14 + FIRST_XOP_ERR
#define ITCCONFIGCHANNEL2_BAD_M                15 + FIRST_XOP_ERR
#define ITCCONFIGCHANNEL2_BAD_A                16 + FIRST_XOP_ERR
#define ITCCONFIGCHANNEL2_BAD_O                17 + FIRST_XOP_ERR
#define ITCCONFIGCHANNEL2_BAD_U                18 + FIRST_XOP_ERR

// checkWave_2D
#define NEED_MIN_ROWS                          19 + FIRST_XOP_ERR

// ITCInitialize2
#define F_FLAG_REQUIRES_ITC18_ITC18USB_ITC1600 20 + FIRST_XOP_ERR
#define D_FLAG_REQUIRES_ITC1600                21 + FIRST_XOP_ERR
#define H_FLAG_REQUIRES_ITC1600                22 + FIRST_XOP_ERR
#define R_FLAG_REQUIRES_ITC1600                23 + FIRST_XOP_ERR

// Trying to use the thread DeviceID when it's not  set
#define THREAD_DEVICE_ID_NOT_SET               24 + FIRST_XOP_ERR

// clang-format on

/* Prototypes */
HOST_IMPORT int XOPMain(IORecHandle ioRecHandle);

// Prototype for CloseAllDevices helper function
void CloseAllDevices();
